#include "main.h"

auto main(int argc, char *argv[]) -> int {
    Args args;
    if (!(parse_arguments(argc, argv, args) && validate_arguments(args))) {
        return 1;
    }

    LOG(INFO) << "Creating new process ...";
    std::unique_ptr<ClangdProcess> clangd_process =
            std::make_unique<ClangdProcess>();
    bool const init =
            clangd_process->Initialize(args.clangd_path, args.compile_commands);
    CHECK(init);

    std::unique_ptr<ClangdClient> clangd =
            std::make_unique<ClangdClient>(std::move(clangd_process));

    LOG(INFO) << "Initializing clangd ...";
    clangd->Initialize(args.root_src, args.compile_commands);

    CodeFileIterator file_iterator(args.root_src);
    file_iterator.Iterate(iterate_code_files_callback, clangd.get());

    LOG(INFO) << "Exiting ...";
    clangd->Exit();

    return 0;
}

auto parse_arguments(int argc, char *argv[], Args &args) -> bool {
    ArgumentParser program = ArgumentParser("ClangSight");

    program.add_argument("-l", "--directory")
            .help("the root directory of the source code");
    program.add_argument("-c", "--commands")
            .help("the directory where \"compile_commands.json\" resides");
    program.add_argument("-d", "--clangd")
            .help("location of the clangd executable");

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        LOG(ERR) << err.what();
        LOG(INFO) << program;
        return false;
    }

    const std::string current_dir = std::filesystem::current_path().string();
    args.root_src = program.present("directory").value_or(current_dir);
    args.compile_commands = program.present("commands").value_or(args.root_src);
    args.clangd_path = program.present("clangd").value_or(
#ifdef WINDOWS
            "clangd.exe"
#else
            "clangd"
#endif
    );

    return true;
}

auto validate_arguments(Args &args) -> bool {
    if (!fs::exists(args.root_src)) {
        LOG(ERR) << "The root directory provided is not valid!";
        return false;
    }
    if (!fs::exists(args.compile_commands)) {
        LOG(ERR) << "The compile_commands path provided is not valid!";
        return false;
    }
    if (!fs::exists(args.clangd_path)) {
        LOG(ERR) << "The clangd path provided is not valid!";
        return false;
    }

    return true;
}

auto iterate_code_files_callback(const std::string &file_path,
                                 const std::string &file_contents,
                                 ClangdClient *clangd) -> void {
    LOG(DEBUG) << "Opening file: " << file_path;

    auto reply = clangd->OpenFile(file_path, file_contents);
    DCHECK(reply.contains("method"));

    reply = clangd->BuildAST(file_path);
    CHECK(reply.contains("result"));
    if (!reply["result"].contains("children")) {
        clangd->CloseFile(file_path);
        return;
    }

    auto root = reply["result"]["children"];

    FunctionDeclarationCallback function_callback = [](const FunctionDeclaration &result) -> void {
        LOG(INFO) << "Function name: " << result.function.name;

        LOG(INFO) << "Function return type: ";
        LOG(INFO) << "\t Name: " << result.return_type.name;
        LOG(INFO) << "\t Type: " << result.return_type.type << "\n";

        if (!result.params.empty()) {
            LOG(INFO) << "Function params: ";
            for (const auto &param: result.params) {
                LOG(INFO) << "\t Name: " << param.name;
                LOG(INFO) << "\t Type: " << param.type;
                LOG(INFO) << "\t Location start: l: " << param.location.start.line << ", c:"
                          << param.location.start.char_index;
                LOG(INFO) << "\t Location end: l: " << param.location.end.line << ", c:"
                          << param.location.end.char_index;
                LOG(INFO) << "\n";
            }
        }

        LOG(INFO) << "================";
    };

    FunctionDeclarationASTVisitor function_visitor(function_callback);
    function_visitor.TraverseAST(root);

    StructDeclarationCallback struct_callback = [](const StructDeclaration &result) -> void {
        LOG(INFO) << result.structure.name;
        LOG(INFO) << result.structure.type;
        LOG(INFO) << "\t Location start: l: " << result.structure.location.start.line << ", c:"
                  << result.structure.location.start.char_index;
        LOG(INFO) << "\t Location end: l: " << result.structure.location.end.line << ", c:"
                  << result.structure.location.end.char_index;
        LOG(INFO) << "================";
    };
    StructDeclarationASTVisitor struct_visitor(struct_callback);
    struct_visitor.TraverseAST(root);

    clangd->CloseFile(file_path);
}