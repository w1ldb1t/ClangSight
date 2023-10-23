#include "clangd_client.h"
#include "clangd/request_database.h"

ClangdClient::ClangdClient(std::unique_ptr<ClangdProcess> clangd_process)
        : request_id(1) {
    clangd_process_ = std::move(clangd_process);
}

auto ClangdClient::flatten_string(const std::string &input) -> std::string {
    std::ostringstream string_builder;
    for (const char character: input) {
        if (character == '"' || character == '\\' || ('\x00' <= character && character <= '\x1f')) {
            string_builder << "\\u"
                           << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(character);
        } else {
            string_builder << character;
        }
    }
    return string_builder.str();
}

auto ClangdClient::Initialize(const std::string &project_path,
                              const std::string &compile_commands_path, bool ast)
-> nlohmann::json {
    std::string msg =
            RequestDatabase::getInstance().getString(REQUEST_INITIALIZE);

    // id
    msg = std::regex_replace(msg, std::regex("\\$ARG0"),
                             std::to_string(request_id++));

    // root of the project
    auto flatten_project_path = flatten_string(project_path);
    msg = std::regex_replace(msg, std::regex("\\$ARG1"), flatten_project_path);

    // enable ast support
    const std::string arg2 = ast ? "true" : "false";
    msg = std::regex_replace(msg, std::regex("\\$ARG2"), arg2);

    // compile commands path
    auto flatten_commands_path = flatten_string(compile_commands_path);
    msg = std::regex_replace(msg, std::regex("\\$ARG3"), flatten_commands_path);

    CHECK(clangd_process_->WriteCommand(msg));
    return clangd_process_->ReadReply();
}

auto ClangdClient::OpenFile(const std::string &file_path, const std::string &contents)
-> nlohmann::json {
    std::string msg = RequestDatabase::getInstance().getString(REQUEST_FILE_OPEN);

    // the file that we want to open
    auto flatten_file_path = flatten_string(file_path);
    msg = std::regex_replace(msg, std::regex("\\$ARG0"), flatten_file_path);

    // the contents of the file
    auto flatten_contents = flatten_string(contents);
    msg = std::regex_replace(msg, std::regex("\\$ARG1"), flatten_contents);

    CHECK(clangd_process_->WriteCommand(msg));
    return clangd_process_->ReadReply();
}

auto ClangdClient::CloseFile(const std::string &file_path) -> nlohmann::json {
    std::string msg = RequestDatabase::getInstance().getString(REQUEST_FILE_CLOSE);

    // file to close
    auto flatten_file_path = flatten_string(file_path);
    msg = std::regex_replace(msg, std::regex("\\$ARG0"), flatten_file_path);

    CHECK(clangd_process_->WriteCommand(msg));
    return clangd_process_->ReadReply();
}

auto ClangdClient::BuildAST(const std::string &file_path) -> nlohmann::json {
    std::string msg = RequestDatabase::getInstance().getString(REQUEST_AST);

    // id
    msg = std::regex_replace(msg, std::regex("\\$ARG0"),
                             std::to_string(request_id++));

    // the file for which we want its AST
    auto flatten_file_path = flatten_string(file_path);
    msg = std::regex_replace(msg, std::regex("\\$ARG1"), flatten_file_path);

    CHECK(clangd_process_->WriteCommand(msg));
    return clangd_process_->ReadReply();
}

auto ClangdClient::Exit() -> void {
    std::string msg = RequestDatabase::getInstance().getString(REQUEST_EXIT);
    CHECK(clangd_process_->WriteCommand(msg));
    CHECK(clangd_process_->ReadReply().empty());
}
