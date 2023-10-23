#ifndef _FUNCTION_AST_VISITOR_H_
#define _FUNCTION_AST_VISITOR_H_
#include "ast_visitor.h"

#include "base/assert.h"
#include "base/logging.h"

struct FunctionDeclaration {
    SymbolType function;
    SymbolType return_type;
    std::vector<SymbolType> params;
};

using FunctionDeclarationCallback = void (*)(const FunctionDeclaration &);

class FunctionDeclarationASTVisitor : public ASTVisitor<FunctionDeclarationCallback>{
public:
    explicit FunctionDeclarationASTVisitor(FunctionDeclarationCallback callback);
private:
    auto TraverseNode(const json &node) -> void override;
};


#endif //_FUNCTION_AST_VISITOR_H_
