#include "request_database.h"

RequestDatabase::RequestDatabase() {
    // Insert strings into the map during the constructor initialization
    request_map_[REQUEST_INITIALIZE] = R"({
      "jsonrpc": "2.0",
      "id": $ARG0,
      "method": "initialize",
      "params": {
          "rootUri": "file:///$ARG1",
          "capabilities": {
              "textDocument": {
                  "ast": {
                      "enabled": $ARG2
                  }
              }
          },
        "initializationOptions": {
          "compilationDatabasePath": "$ARG3"
        }
      }
    })";

    request_map_[REQUEST_FILE_OPEN] = R"({
    "jsonrpc": "2.0",
    "method": "textDocument/didOpen",
    "params": {
      "textDocument": {
        "uri": "file:///$ARG0",
        "languageId": "cpp",
        "version": 1,
        "text": "$ARG1"
      }
    }
  })";

    request_map_[REQUEST_FILE_CLOSE] = R"({
    "jsonrpc": "2.0",
    "method": "textDocument/didClose",
    "params": {
        "textDocument": {
          "uri": "file:///$ARG0"
        }
    }
  })";

    request_map_[REQUEST_AST] = R"({
    "jsonrpc":"2.0",
    "id": $ARG0,
    "method":"textDocument/ast",
    "params":{
        "textDocument":{
          "uri":"file:///$ARG1"
        }
    }
  })";

    request_map_[REQUEST_EXIT] = R"({
    "jsonrpc": "2.0",
    "method": "exit"
  })";
}

auto RequestDatabase::getInstance() -> RequestDatabase & {
    static RequestDatabase instance;
    return instance;
}

auto RequestDatabase::getString(RequestKey key) const -> std::string {
    return request_map_.at(key);
}