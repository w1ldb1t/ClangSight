#ifndef _MISC_
#define _MISC_

#include <string>
#include <sstream>
#include <vector>

template<typename... Args>
auto concat(const Args&... args) -> std::string
{
    std::ostringstream oss; // NOLINT(*-const-correctness)
    (oss << ... << args);
    return oss.str();
}

inline auto remove_lines(std::string& str, int linesToRemove) -> void {
    auto newline_pos = str.find('\n');
    while (newline_pos != std::string::npos && linesToRemove > 0) {
        str.erase(0, newline_pos + 1);
        --linesToRemove;
        newline_pos = str.find('\n');
    }
}

inline auto split_string(const std::string& input, char delimiter) -> std::vector<std::string> {
    std::vector<std::string> result;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}

#endif // _MISC_