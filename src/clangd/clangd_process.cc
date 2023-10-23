#include "clangd_process.h"

auto ClangdProcess::Initialize(std::string &clangd_path,
                               std::string &compile_commands_path) -> bool {
    DCHECK(!process_handle_->IsRunning());

    std::string clangd_cmd = concat(clangd_path, " ");
    clangd_cmd = concat(clangd_cmd, "--background-index", " ");
    clangd_cmd =
            concat(clangd_cmd, "--compile-commands-dir=", compile_commands_path, " ");
    clangd_cmd = concat(clangd_cmd, "--log=error");

    return process_handle_->Start(clangd_cmd);
}

auto ClangdProcess::ReadReply() -> nlohmann::json {
    auto reply = last_reply_;
    last_reply_.clear();
    return reply;
}

auto ClangdProcess::WriteCommand(std::string &request) -> bool {
    const size_t request_size = request.size();
    DCHECK_GT(request_size, 0U);

    std::stringstream str_stream;
    str_stream << "Content-Length: ";
    str_stream << request_size;
    str_stream << "\r\n\r\n";
    const std::string header = str_stream.str();

    // Build a request with header + request
    std::string full_request;
    full_request.append(header);
    full_request.append(request);

    // clear any previous response
    last_reply_.clear();

    // Write the command to the client
    const bool result = process_handle_->WriteConsoleText(full_request);
    if (result) {
        // parse and save the reply in case we need to return it
        std::string response = process_handle_->ReadConsoleText();
        if (!response.empty()) {
            // remove the first two lines, because they
            // contain the content-length of clangd's reply
            remove_lines(response, 2);

            try {
                last_reply_ = nlohmann::json::parse(response);
            } catch (nlohmann::json::exception &ex) {
                LOG(ERR) << ex.what();
            }
        }
    }
    return result;
}