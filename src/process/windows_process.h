#ifndef _WINDOWS_PROCESS_H_
#define _WINDOWS_PROCESS_H_

#include "process.h"

#include "base/assert.h"

#include <windows.h>

class WindowsProcess : public Process {
public:
  WindowsProcess() noexcept;
  ~WindowsProcess() override;

  auto Start(const std::string &command) -> bool override;
  auto IsRunning() -> bool override;
  auto ReadConsoleText() -> std::string override;
  auto WriteConsoleText(const std::string &input) -> bool override;
  auto IsInitialized() -> bool override;
  auto Stop() -> void override;

private:
  HANDLE process_handle_;
  HANDLE stdin_write_handle_;
  HANDLE stdout_read_handle_;
};

#endif //_WINDOWS_PROCESS_H_