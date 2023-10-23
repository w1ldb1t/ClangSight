#include "posix_process.h"

PosixProcess::PosixProcess() noexcept
    : process_id_(-1), stdin_pipe_(-1), stdout_pipe_(-1) {}

PosixProcess::~PosixProcess() {
  if (!this->IsInitialized()) {
    return;
  }

  close(stdin_pipe_);
  close(stdout_pipe_);

  if (this->IsRunning()) {
    this->Stop();
  }
}

auto PosixProcess::Start(const std::string &command) -> bool {
  int pipe_out[2];
  int pipe_in[2];
  int exit_code;
  posix_spawn_file_actions_t action;

  // Create stdin/stdout pipes
  if (pipe(pipe_in) || pipe(pipe_out)) {
    LOG(DEBUG) << "Failed to create stdin/stdout pipes.";
    return false;
  }
  // List the file operations to be performed on the process
  if (posix_spawn_file_actions_init(&action) != 0) {
    LOG(DEBUG) << "posix_spawn_file_actions_init failed!";
    return false;
  }
  // Close the unused read end of the pipe
  if (posix_spawn_file_actions_addclose(&action, pipe_in[1]) != 0) {
    LOG(DEBUG) << "posix_spawn_file_actions_addclose failed!";
    return false;
  }
  // Close the unused write end of the pipe
  if (posix_spawn_file_actions_addclose(&action, pipe_out[0]) != 0) {
    LOG(DEBUG) << "posix_spawn_file_actions_addclose failed!";
    return false;
  }
  // Redirect child stdin to our pipe
  if (posix_spawn_file_actions_adddup2(&action, pipe_in[0], STDIN_FILENO) !=
      0) {
    LOG(DEBUG) << "posix_spawn_file_actions_adddup2 failed!";
    return false;
  }
  // Redirect child stdout to our pipe
  if (posix_spawn_file_actions_adddup2(&action, pipe_out[1], STDOUT_FILENO) !=
      0) {
    LOG(DEBUG) << "posix_spawn_file_actions_adddup2 failed!";
    return false;
  }
  // Silence any clangd error messages.
  // TODO: Remove this once I/O error issue is resolved
  if (posix_spawn_file_actions_addopen(&action, STDERR_FILENO, "/dev/null",
                                       O_WRONLY, 0) != 0) {
    LOG(DEBUG) << "posix_spawn_file_actions_adddup2 failed!";
    return false;
  }

  // Create the argument list for the child process
  auto args = split_string(command, ' ');
  std::vector<char *> argv;
  argv.reserve(args.size());
  for (auto &arg : args) {
    argv.push_back(&arg[0]);
  }
  argv.push_back(nullptr);

  pid_t pid;
  if (posix_spawnp(&pid, argv[0], &action, NULL, &argv[0], NULL) != 0) {
    LOG(DEBUG) << "posix_spawnp failed with error: " << strerror(errno);
    return false;
  }

  // close child-side of pipes
  close(pipe_out[1]);
  close(pipe_in[0]);

  process_id_ = pid;
  stdin_pipe_ = pipe_in[1];
  stdout_pipe_ = pipe_out[0];
  return true;
}

auto PosixProcess::ReadConsoleText() -> std::string {
  std::vector<pollfd> plist = {{stdout_pipe_, POLLIN}};
  std::stringstream out_data;
  int rval = poll(&plist[0], plist.size(), /*timeout*/ -1);
  std::string buffer(4096, ' ');

  if (plist[0].revents & POLLIN) {
    for (int bytes_read;
         (bytes_read = read(stdout_pipe_, buffer.data(), buffer.length()));) {
      out_data << buffer.substr(0, static_cast<size_t>(bytes_read));
      if (bytes_read == 0 || bytes_read != buffer.length()) {
        break;
      }
    }
  }

  return out_data.str();
}

auto PosixProcess::WriteConsoleText(const std::string &input) -> bool {
  int bytes_written = write(stdin_pipe_, input.c_str(), input.size());
  CHECK_NE(bytes_written, -1);

  if (bytes_written > 0) {
    LOG(DEBUG) << "Successfully wrote to pipe (" << bytes_written << ")";
    return true;
  }
  return false;
}

auto PosixProcess::IsRunning() -> bool {
  DIR *proc_dir = opendir("/proc");
  if (!proc_dir) {
    LOG(DEBUG) << "Failed to open /proc directory";
    return false;
  }

  dirent *entry;
  while ((entry = readdir(proc_dir))) {
    if (entry->d_type == DT_DIR && std::isdigit(entry->d_name[0])) {
      pid_t pid = std::stoi(entry->d_name);
      if (pid == process_id_) {
        closedir(proc_dir);
        return true;
      }
    }
  }

  closedir(proc_dir);
  return false;
}

auto PosixProcess::IsInitialized() -> bool {
  return process_id_ != -1;
}

auto PosixProcess::Stop() -> void {
  int result = kill(process_id_, SIGTERM);
  DCHECK_EQ(result, 0);
}