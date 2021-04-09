#pragma once
#include <unordered_map>
#include <iostream>
#include "record.h"

struct Symbol {
    Record type;
    Record id;
};

class SymbolTable {
private:
    unordered_map<string, Symbol> table;
public:
    SymbolTable();

    bool add(const Record & type, const Record & identifier) {
        if (exists(identifier.lexeme)) {
            return false;
        } else {
            Symbol symbol = {type, identifier};
            table.insert(make_pair(identifier.lexeme, symbol));
            return true;
        }
    }

    bool exists(const string & identifier) const {
        return table.find(identifier) != table.end();
    }

    string toString() const;
};

//
// outputs the string representation of a record to an output stream
//
inline ostream & operator << (ostream & stream, const SymbolTable & table){
    stream << table.toString() << endl;
    return stream;
}