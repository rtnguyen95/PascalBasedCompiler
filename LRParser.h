//
// Created by HashEngineering on 4/30/2021.
//

#ifndef _323_LRPARSER_H
#define _323_LRPARSER_H
#include <stack>
#include <map>
#include "syntaxanalyzer.h"
#include "record.h"

#define RULE(x) {x, "<" #x ">"}

using namespace::std;
struct LREntry {
    enum Operation {
        Shift,
        Reduce,
        Accept,
        Error,
        Goto,
        Token,
        Production,
        EndOfFile
    };
    Operation operation;
    int value;
    string production;
    Record token;

    LREntry(Operation operation) : operation(operation), token("", "", true, "", 0, 0, "") {

    }

    LREntry(Operation operation, int value) : operation(operation), value(value), token("", "", true, "", 0, 0, "") {

    }

    LREntry(const Record & token) : operation(Token), value (-1), token(token) {

    }

    LREntry(const string & production, int number) : operation(Production), production(production), value(number), token("", "", true, "", 0, 0, "") {

    }

    bool operator==(const LREntry & other) const {
        return operation == other.operation && value == other.value && production == other.production;
    }

    bool operator!=(const LREntry & other) const {
        return !operator==(other);
    }

    bool operator<(const LREntry & other) const {
        return production.compare(other.production) < 0;
    }
};

struct ReduceEntry : public LREntry {
    explicit ReduceEntry(int rule) : LREntry(Reduce, rule) {}
};

struct ShiftEntry : public LREntry {
    explicit ShiftEntry(int state) : LREntry(Shift, state) {}
};

struct GotoEntry : public LREntry {
    explicit GotoEntry(int state) : LREntry(Goto, state) {}
};

class LRParser : public SyntaxAnalyzer {

    enum Productions {
        Expression = 6,
        Term,
        Factor
    };

    enum Terminals {
        Identifier = 0,
        Addition = 1,
        Subtraction = 2,
        Multiply = 3,
        Divide = 4,
        LeftParen = 5,
        RightParen = 6,
        EndOfFile = 7,
        Unknown = 8
    };

    map<string, int> productionNames = {
            { "E", Expression },
            { "T", Term },
            { "F", Factor}
    };

    list<LREntry> productionStack;
    stack<Node *> parseTreeStack;
    static LREntry table[16][11];
    static list<pair<LREntry, string>> productions;
public:
    LRParser(LexicalScanner & lexicalScanner, SymbolTable & symbolTable, ErrorHandler & errorHandler);

    ParseTree * createParseTree();
    bool stackProcess();

    int columnFromToken(Record & token);
    int columnFromRecord(LREntry & record);
    int rowFromRecord(LREntry & record);

    //int isTerminal(LREntry & entry) {
    //    return record.token.lexeme != "";
    //}

    string to_string(LREntry item) {
        switch (item.operation) {
            case LREntry::Token:
                return item.token.lexeme;
            case LREntry::Error:
                return "Error";
            case LREntry::Shift:
                return string("S") + ::to_string(item.value);
            case LREntry::Reduce:
                return string("R") + ::to_string(item.value);
            case LREntry::Goto:
                return ::to_string(item.value);
            case LREntry::Accept:
                return "Accept";
            case LREntry::EndOfFile:
                return "$";
            case LREntry::Production:
                return item.production;
            default:
                return "Unknown";
        }
    }

    LREntry getTopMostTerminal();
    int getProductionTerminalCount(int rule);
    LREntry getProduction(int rule);
    string getProductionRHS(int rule);

    string getStackAsString();

    void printIntermediateCode(ostream & stream);
    void printIntermediateCode(ostream & stream, Node * node);
};


#endif //_323_OPERATORPRECEDENCEPARSER_H
