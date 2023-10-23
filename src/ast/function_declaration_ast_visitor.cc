#include "function_declaration_ast_visitor.h"

auto FunctionDeclarationASTVisitor::TraverseNode(const json &node) -> void {
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

    if ((arcana.starts_with("FunctionDecl") || arcana.starts_with("CXXMethodDecl")) &&
        (kind == "CXXMethod" || kind == "Function") &&
        // We assume that operator overloading shouldn't be considered a func
        (detail.find("operator") == std::string::npos)) {

        FunctionDeclaration result;
        result.function.name = detail;
        result.function.location = ReadRange(node);
        result.function.type = node["kind"].get<std::string>();

        // Set |result.return_type| & |result.params|
        if (node.contains("children") && !node["children"].empty()) {
            auto full_function_details = node["children"][0];
            if (full_function_details.contains("children") && !full_function_details["children"].empty()) {
                auto function_info = full_function_details["children"];
                DCHECK_GT(function_info.size(), 0);

                // return type
                auto return_type_node = function_info[0];
                const std::string return_type_arcana = return_type_node["arcana"].get<std::string>();
                result.return_type.name = ReadArcanaType(return_type_arcana);
                result.return_type.type = return_type_node["kind"].get<std::string>();
                if (return_type_node.contains("range")) {
                    result.return_type.location = ReadRange(return_type_node);
                }

                // arguments
                if (function_info.size() > 1) {
                    for (json::iterator it = ++function_info.begin(); it != function_info.end(); ++it) {
                        auto param_elm_container = it.value();

                        const std::string param_elm_arcana = param_elm_container["arcana"].get<std::string>();
                        if (!param_elm_arcana.starts_with("ParmVarDecl")) {
                            continue;
                        }
                        CHECK(param_elm_container.contains("children"));
                        DCHECK(param_elm_container.contains("detail"));

                        // get the parameter's (var) name
                        auto param_name = param_elm_container["detail"].get<std::string>();

                        // grab the first child of |param_elm_container| which should
                        // contain basic information for that specific variable
                        auto param_obj = param_elm_container["children"][0];
                        auto param_arcana = param_obj["arcana"].get<std::string>();

                        auto param_type = ReadArcanaType(param_arcana);
                        auto param_range = ReadRange(param_obj);
                        DCHECK_NE(param_type.size(), 0);

                        SymbolType new_param;
                        new_param.name = param_name;
                        new_param.location = param_range;
                        new_param.type = param_type;

                        result.params.push_back(new_param);
                    }
                }
            }
        }

        // Notify
        callback_(result);
    }
}

FunctionDeclarationASTVisitor::FunctionDeclarationASTVisitor(FunctionDeclarationCallback callback) : ASTVisitor(
        callback) {
}
