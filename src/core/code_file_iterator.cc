#include "code_file_iterator.h"

auto CodeFileIterator::ReadFileContents(const std::string &file_path) -> std::string {
    try {
        // Check if the file exists and is a regular file
        if (!fs::is_regular_file(file_path)) {
            throw std::runtime_error("File not found or not a regular file: " +
                                     file_path);
        }

        // Open the file using an input file stream
        std::ifstream file_stream(file_path);

        // Read the entire file into a string
        std::string file_contents((std::istreambuf_iterator<char>(file_stream)),
                                  std::istreambuf_iterator<char>());

        // The file stream will be automatically closed when it goes out of scope
        return file_contents;
    } catch (const std::exception &ex) {
        std::cerr << "Error accessing the file: " << ex.what() << '\n';
        return "";
    }
}