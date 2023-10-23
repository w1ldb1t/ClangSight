#include "windows_process.h"

WindowsProcess::WindowsProcess() noexcept
        : process_handle_(INVALID_HANDLE_VALUE),
          stdin_write_handle_(INVALID_HANDLE_VALUE),
          stdout_read_handle_(INVALID_HANDLE_VALUE) {}

WindowsProcess::~WindowsProcess() {
    if (!this->IsInitialized()) {
        return;
    }

    CloseHandle(stdin_write_handle_);
    CloseHandle(stdout_read_handle_);

    if (this->IsRunning()) {
        this->Stop();
    }
}

auto WindowsProcess::Start(const std::string &command) -> bool {
    // Create pipe handles for stdin and stdout
    SECURITY_ATTRIBUTES s_attrs;
    s_attrs.nLength = sizeof(SECURITY_ATTRIBUTES);
    s_attrs.lpSecurityDescriptor = nullptr;
    s_attrs.bInheritHandle = TRUE;

    HANDLE stdin_read_handle;
    HANDLE stdout_write_handle;

    // Create the pipe for stdin
    if (CreatePipe(&stdin_read_handle, &stdin_write_handle_, &s_attrs, 0) == 0) {
        LOG(DEBUG) << "Failed to create stdin pipe.";
        return false;
    }

    // Create the pipe for stdout
    if (CreatePipe(&stdout_read_handle_, &stdout_write_handle, &s_attrs, 0) ==
        0) {
        LOG(DEBUG) << "Failed to create stdout pipe.";
        return false;
    }

    // Set the pipe mode to non-blocking (useful for reads later)
    DWORD pipe_mode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
    if (SetNamedPipeHandleState(stdout_read_handle_, &pipe_mode, nullptr,
                                nullptr) == 0) {
        LOG(DEBUG) << "Failed to set pipe mode for the pipe handle.";
        return false;
    }

    // Set up startup information for the process
    STARTUPINFO s_info;
    ZeroMemory(&s_info, sizeof(STARTUPINFO));
    s_info.cb = sizeof(STARTUPINFO);
    s_info.hStdError = stdout_write_handle;
    s_info.hStdOutput = stdout_write_handle;
    s_info.hStdInput = stdin_read_handle;
    s_info.dwFlags |= STARTF_USESTDHANDLES;

    // Create process with redirected stdin and stdout
    PROCESS_INFORMATION p_info;
    ZeroMemory(&p_info, sizeof(PROCESS_INFORMATION));

    if (!CreateProcess(nullptr, const_cast<LPSTR>(command.c_str()), nullptr,
                       nullptr, TRUE, 0, nullptr, nullptr, &s_info, &p_info)) {
        LOG(DEBUG) << "Failed to start process.";
        return false;
    }

    process_handle_ = p_info.hProcess;

    // Close unused pipe handles
    CloseHandle(stdin_read_handle);
    CloseHandle(stdout_write_handle);
    return true;
}

auto WindowsProcess::ReadConsoleText() -> std::string {
    DWORD bytes_read;
    std::stringstream out_data;
    std::string buffer(4096, ' ');

    for (;;) {
        BOOL ret = ReadFile(stdout_read_handle_, buffer.data(), static_cast<DWORD>(buffer.length()),
                            &bytes_read, nullptr);
        if (ret == FALSE) {
            int error = static_cast<int>(GetLastError());
            // Since the pipe is set as "non-blocking", it may falsely return
            // ERROR_NO_DATA while the pipe is termporarily empty.
            if (error == ERROR_NO_DATA) {
                continue;
            }
            // Process has most probably died/exit
            // so abort the function immediately
            if (error == ERROR_BROKEN_PIPE) {
                return "";
            }
            LOG(ERR) << std::system_category().message(error);
            NOT_REACHED();
        }

        out_data << buffer.substr(0, static_cast<size_t>(bytes_read));
        if (bytes_read == 0 || bytes_read != buffer.length()) {
            break;
        }
    }

    return out_data.str();
}

auto WindowsProcess::WriteConsoleText(const std::string &input) -> bool {
    DWORD bytes_written;
    if (WriteFile(stdin_write_handle_, input.c_str(), static_cast<DWORD>(input.size()),
                  &bytes_written, nullptr) != 0) {
        DCHECK_GT(bytes_written, 0U);
        LOG(DEBUG) << "Successfully wrote to pipe (" << bytes_written << ")";
        BOOL flush = FlushFileBuffers(stdin_write_handle_);
        return flush == TRUE;
    }

    // failed to write to pipe
    const int error = static_cast<int>(GetLastError());
    LOG(DEBUG) << std::system_category().message(error);
    return false;
}

auto WindowsProcess::IsRunning() -> bool {
    if (process_handle_ == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD exit_code;
    GetExitCodeProcess(process_handle_, &exit_code);
    return exit_code == STILL_ACTIVE;
}

auto WindowsProcess::IsInitialized() -> bool {
    return process_handle_ != INVALID_HANDLE_VALUE;
}

auto WindowsProcess::Stop() -> void {
    BOOL ret = TerminateProcess(process_handle_, 0);
    DCHECK_NE(ret, 0);
}