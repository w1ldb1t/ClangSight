//
// Created by Michelangelo on 8/26/2023.
//

#ifndef _AST_MISC_H_
#define _AST_MISC_H_

#include "ast_structs.h"

#include "base/assert.h"

#include "nlohmann/json.hpp"

auto ReadRange(const nlohmann::json &node) -> SymbolBounds;
auto ReadArcanaType(const std::string& input) -> std::string;

#endif //_AST_MISC_H_
