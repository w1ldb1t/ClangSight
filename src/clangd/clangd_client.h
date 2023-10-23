#ifndef _CLANGD_CLIENT_H_
#define _CLANGD_CLIENT_H_

#include "clangd_process.h"

#include "request_database.h"

#include "nlohmann/json.hpp"

#include <regex>

class ClangdClient {
public:
  explicit ClangdClient(std::unique_ptr<ClangdProcess> clangd_process);

  // The msgs we can send to clangd
  auto Initialize(const std::string& project_path, const std::string& compile_commands_path, bool ast = true) -> nlohmann::json;
  auto OpenFile(const std::string& file_path, const std::string& contents) -> nlohmann::json;
  auto CloseFile(const std::string& file_path) -> nlohmann::json;
  auto BuildAST(const std::string& file_path) -> nlohmann::json;
  auto Exit() -> void;
  // ...

private:
  static auto flatten_string(const std::string& input) -> std::string;

  uint64_t request_id;
  std::unique_ptr<ClangdProcess> clangd_process_;
};

#endif //_CLANGD_CLIENT_H_