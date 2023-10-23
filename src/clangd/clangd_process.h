#ifndef _CLANGD_PROCESS_H_
#define _CLANGD_PROCESS_H_
#include "process/process_handle.h"

#include "base/logging.h"
#include "base/assert.h"
#include "base/misc.h"

#include "nlohmann/json.hpp"

#include <memory>

class ClangdProcess {
public:
  ClangdProcess() = default;
  ~ClangdProcess() = default;

  auto Initialize(std::string& clangd_path, std::string& compile_commands_path) -> bool;
  auto ReadReply() -> nlohmann::json;
  auto WriteCommand(std::string& request) -> bool;

private:
  ProcessHandle process_handle_;
  nlohmann::json last_reply_;
};

#endif //_CLANGD_PROCESS_H_