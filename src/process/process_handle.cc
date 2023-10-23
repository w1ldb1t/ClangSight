#include "process_handle.h"

ProcessHandle::ProcessHandle() {
#if defined(WINDOWS)
  impl_ = new WindowsProcess();
#elif defined(POSIX)
  impl_ = new PosixProcess();
#endif
}

ProcessHandle::ProcessHandle(Process *process) : impl_(process) {}

ProcessHandle::~ProcessHandle() { delete impl_; }

ProcessHandle::ProcessHandle(ProcessHandle &&other) noexcept
    : impl_(other.impl_) {
  other.impl_ = nullptr;
}

auto ProcessHandle::operator=(ProcessHandle &&rhs) noexcept -> ProcessHandle & {
  std::swap(impl_, rhs.impl_);
  return *this;
}

auto ProcessHandle::reset() -> void {
  if (impl_ != nullptr) {
    delete impl_;
    impl_ = nullptr;
  }
}