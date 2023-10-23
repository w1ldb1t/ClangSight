#ifndef _AST_VISITOR_H_
#define _AST_VISITOR_H_
#include "nlohmann/json.hpp"

#include "ast_structs.h"
#include "ast_misc.h"

using json = nlohmann::json;

template <typename T>
class ASTVisitor {
public:
    explicit ASTVisitor(T callback);

    auto TraverseAST(const json &ast) -> void;

protected:
    virtual void TraverseNode(const json &node) = 0;

    T callback_;
};

template<typename T>
ASTVisitor<T>::ASTVisitor(T callback) {
    callback_ = callback;
}

template<typename T>
auto ASTVisitor<T>::TraverseAST(const json &ast) -> void {
    // We assume that the json obj is an array of objects
    if (ast.is_array()) {
        for (const auto &node: ast) {
            TraverseNode(node);

            // Traverse children (if any)
            if (node.contains("children")) {
                const json &children = node["children"];
                if (children.is_array()) {
                    for (const auto &child: children) {
                        TraverseNode(child);
                    }
                }
            }
        }
    }
}

#endif//_AST_VISITOR_H_