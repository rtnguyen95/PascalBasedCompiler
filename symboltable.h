#pragma once
#include <unordered_map>
#include <iostream>
#include "record.h"

/*
 The symbol table class manages a list of identifiers (or symbols) found by the Syntax analyzer.
 The table keeps track of the list of symbols with their types.  Both are represented as Records.
 
Example of what is stored in SymbolTable after being run on source code
 
 Source code:
 float a
 int b
 bool c
 
 SymbolTable:
 (Type)            (Identifier)                     (Line Number)
 float                  a                              (1)
 int                    b                              (2)
 bool                   c                              (3)
 */

struct Symbol {
    Record type;
    Record id;
};

class SymbolTable
{
private:
    unordered_map<string, Symbol> table; //unordered map for holding the identifier and its corresponding Symbol data
public:
    SymbolTable(); //default constructor

    //The add function will add the symbol to the table. It accepts two arguments, a Record object with the data of the type and a Record object with the data of the identifier. It returns a boolean, true if the add was successful, false if otherwise
    bool add(const Record & type, const Record & identifier) {
        //checks to see if the symbol was previously added to the symbol table. If true, the symbol is not added to the table and the function returns false
        if (exists(identifier.lexeme)) {
            return false;
            //else the symbol is created as a Symbol, added to table, and the function returns true to indicate the success of the add
        } else {
            Symbol symbol = {type, identifier};
            table.insert(make_pair(identifier.lexeme, symbol));
            return true;
        }
    }

    // The function exists will check to see if symbol or identifier was previously added to the symbol table. It accepts a single argument of a string representing the identifier being searched for and checks the entire table for the identifier. The function returns a boolean, true if the identifier exists in the symbol table, false if otherwise 
    bool exists(const string & identifier) const {
        return table.find(identifier) != table.end();
    }

    string getType(const string & identifier) const {
        return exists(identifier) ? table.find(identifier)->second.type.lexeme : "";
    }

    string toString() const;
};

// outputs the string representation of a record to an output stream
inline ostream & operator << (ostream & stream, const SymbolTable & table){
    stream << table.toString() << endl;
    return stream;
}
