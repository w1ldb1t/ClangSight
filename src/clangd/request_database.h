#ifndef _REQUEST_DATABASE_H_
#define _REQUEST_DATABASE_H_

#include <string>
#include <unordered_map>

enum RequestKey {
    REQUEST_INITIALIZE,
    REQUEST_FILE_OPEN,
    REQUEST_FILE_CLOSE,
    REQUEST_AST,
    REQUEST_EXIT,
};

class RequestDatabase {
public:
    static auto getInstance() -> RequestDatabase&;
    [[nodiscard]] auto getString(RequestKey key) const -> std::string;

private:
    RequestDatabase();
    ~RequestDatabase() = default;

    std::unordered_map<RequestKey, std::string> request_map_;
};

#endif//_REQUEST_DATABASE_H_