#include "symboltable.h"

SymbolTable::SymbolTable() {

}

string SymbolTable::toString() const {
    string result = "SYMBOL TABLE\n\n";
    for (auto s: table) {
        result.append(s.first).append(": ").append(s.second.type.lexeme).append("\n");
    }
    return result;
}
