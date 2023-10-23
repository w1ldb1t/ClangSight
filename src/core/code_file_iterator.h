#ifndef _CODEFILEITERATOR_H_
#define _CODEFILEITERATOR_H_

#include "base/logging.h"

#include <utility>
#include <vector>
#include <fstream>
#include <filesystem>
#include <functional>

namespace fs = std::filesystem;

class CodeFileIterator {
public:
    explicit CodeFileIterator(fs::path directory)
            : directory_(std::move(directory)) {}

    template <typename... Args>
    using CodeFileIteratorCallback = void (*)(const std::string &, const std::string &, Args...);

    template<typename... Args>
    auto Iterate(CodeFileIteratorCallback<Args...> callback, Args... args) -> void {
        for (const auto& entry : fs::recursive_directory_iterator(
                directory_, fs::directory_options::skip_permission_denied)) {
            try {
                if (!entry.is_regular_file()) {
                    continue;
                }

                const fs::path& file_path = entry.path();
                const std::string& file_extension = file_path.extension().string();

                if (file_extension != ".c" && file_extension != ".cpp" &&
                    file_extension != ".cc" && file_extension != ".h" &&
                    file_extension != ".hpp") {
                    continue;
                }

                const std::string file_contents = ReadFileContents(file_path.string());

                callback(file_path.string(), file_contents, args...);
            } catch (const fs::filesystem_error& ex) {
                LOG(ERR) << ex.what() << '\n';
                continue;
            }
        }
    }

private:
    static auto ReadFileContents(const std::string &file_path) -> std::string;

    fs::path directory_;
};

#endif //_CODEFILEITERATOR_H_
