#ifndef _STRUCT_AST_VISITOR_H_
#define _STRUCT_AST_VISITOR_H_
#include "ast_visitor.h"

#include "base/assert.h"
#include "base/logging.h"

struct StructDeclaration {
    SymbolType structure;
};

using StructDeclarationCallback = void (*)(const StructDeclaration &);

class StructDeclarationASTVisitor : public ASTVisitor<StructDeclarationCallback>{
public:
    explicit StructDeclarationASTVisitor(StructDeclarationCallback callback);
private:
    auto TraverseNode(const json &node) -> void override;
};

#endif//_STRUCT_AST_VISITOR_H_
