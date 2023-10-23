#ifndef _PROCESS_HANDLE_H_
#define _PROCESS_HANDLE_H_

#include "base/misc.h"
#include "base/platform.h"

#if defined(WINDOWS)
#include "windows_process.h"
#elif defined(POSIX)
#include "posix_process.h"
#endif

class ProcessHandle final {
public:
  explicit ProcessHandle();
  explicit ProcessHandle(Process *process);
  ~ProcessHandle();
  
  ProcessHandle(ProcessHandle &&other) noexcept;
  auto operator=(ProcessHandle &&rhs) noexcept -> ProcessHandle &;

  auto operator->() -> Process * { return impl_; }
  auto operator->() const -> Process * { return impl_; }
  explicit operator bool() const { return impl_ != nullptr; }

  auto reset() -> void;

private:
  Process *impl_;
};

#endif //_PROCESS_HANDLE_H_