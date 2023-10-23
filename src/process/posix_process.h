#ifndef _POSIX_PROCESS_H_
#define _POSIX_PROCESS_H_

#include "base/misc.h"
#include "process.h"
#include "base/assert.h"

#include <poll.h>
#include <spawn.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>

class PosixProcess : public Process {
public:
  PosixProcess() noexcept;
  ~PosixProcess() override;

  auto Start(const std::string &command) -> bool override;
  auto IsRunning() -> bool override;
  auto ReadConsoleText() -> std::string override;
  auto WriteConsoleText(const std::string &input) -> bool override;
  auto IsInitialized() -> bool override;
  auto Stop() -> void override;

private:
  pid_t process_id_;
  int stdin_pipe_;
  int stdout_pipe_;
};

#endif //_POSIX_PROCESS_H_