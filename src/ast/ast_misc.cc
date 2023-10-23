#include "ast_misc.h"

auto ReadRange(const nlohmann::json &node) -> SymbolBounds {
    auto range_loc = node["range"];
    DCHECK(range_loc.contains("start"));
    DCHECK(range_loc.contains("end"));

    auto start_loc = range_loc["start"];
    DCHECK(start_loc.contains("character"));
    DCHECK(start_loc.contains("line"));

    auto end_loc = range_loc["end"];
    DCHECK(end_loc.contains("character"));
    DCHECK(end_loc.contains("line"));

    auto start_char_idx = start_loc["character"].get<int>();
    auto end_char_idx = end_loc["character"].get<int>();

    auto start_line_idx = start_loc["line"].get<int>();
    auto end_line_idx = end_loc["line"].get<int>();

    SymbolBounds result;

    result.start.line = start_line_idx;
    result.start.char_index = start_char_idx;

    result.end.line = end_line_idx;
    result.end.char_index = end_char_idx;

    return result;
}

auto ReadArcanaType(const std::string &input) -> std::string {
    bool inside_quotes = false;
    std::string current_token;

    for (const char character: input) {
        if (character == '\'') {
            inside_quotes = !inside_quotes;
            if (!inside_quotes && !current_token.empty()) {
                return current_token;
            }
        } else if (inside_quotes) {
            current_token += character;
        }
    }

    return "";  // Return an empty string if there is no 'type'
}