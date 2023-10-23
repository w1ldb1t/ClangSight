#include "struct_declaration_ast_visitor.h"

auto StructDeclarationASTVisitor::TraverseNode(const json &node) -> void {
    if (!node.contains("arcana")) {
        return;
    }

    std::string arcana;
    std::string kind;
    std::string detail;

    arcana = node["arcana"].get<std::string>();

    if (node.contains("kind")) {
        kind = node["kind"].get<std::string>();
    }
    if (node.contains("detail")) {
        detail = node["detail"].get<std::string>();
    }

    if (arcana.starts_with("CXXRecordDecl") && arcana.find(" struct ") != std::string::npos) {
        DCHECK(node.contains("range"));

        StructDeclaration result;
        result.structure.name = detail;
        result.structure.location = ReadRange(node);
        result.structure.type = kind;

        callback_(result);
    }
}

StructDeclarationASTVisitor::StructDeclarationASTVisitor(StructDeclarationCallback callback) : ASTVisitor(callback) {
}
