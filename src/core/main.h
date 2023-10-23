#include "base/logging.h"
#include "base/assert.h"
#include "base/misc.h"
#include "base/platform.h"

#include "ast/function_declaration_ast_visitor.h"
#include "ast/struct_declaration_ast_visitor.h"

#include "clangd/clangd_client.h"

#include "code_file_iterator.h"

#include "argparse/argparse.hpp"

#include <fstream>
#include <filesystem>

using json = nlohmann::json;
using ArgumentParser = argparse::ArgumentParser;

struct Args {
    std::string root_src;
    std::string compile_commands;
    std::string clangd_path;
};

auto main(int argc, char *argv[]) -> int;

auto parse_arguments(int argc, char *argv[], Args &args) -> bool;

auto validate_arguments(Args &args) -> bool;

auto iterate_code_files_callback(const std::string &file_path,
                                 const std::string &file_contents,
                                 ClangdClient *clangd) -> void;