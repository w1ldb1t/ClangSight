#ifndef _AST_STRUCTS_H_
#define _AST_STRUCTS_H_

#include <string>

struct DocumentLocation {
    int line;
    int char_index;
};

struct SymbolBounds {
    DocumentLocation start;
    DocumentLocation end;
};

struct SymbolType {
    std::string name; // detail?, i.e 'int'
    std::string type; // kind?, i.e 'Builtin'
    SymbolBounds location;
};

#endif //_AST_STRUCTS_H_
