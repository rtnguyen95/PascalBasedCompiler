#include "symboltable.h"

SymbolTable::SymbolTable() {

}

string SymbolTable::toString() const {
    string result = "SYMBOL TABLE\n\n";
    for (auto s: table) {
        result.append(s.first).append(": ").append(s.second.type.lexeme).append("\t")
            .append(*s.second.id.filename).append(":").append(to_string(s.second.id.line))
            .append(":").append(to_string(s.second.id.linePosition))
            .append("\n");
    }
    return result;
}
