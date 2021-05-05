//
// Created by HashEngineering on 4/30/2021.
//

#ifndef _323_OPERATORPRECEDENCEPARSER_H
#define _323_OPERATORPRECEDENCEPARSER_H
#include <stack>
#include <map>
#include "syntaxanalyzer.h"
#include "record.h"
using namespace::std;
struct OperatorTableRecord {
    string value;
    Record token;

    OperatorTableRecord(const string & name) : value(name), token("", "", true, "", 0, 0, "") {

    }

    OperatorTableRecord(const string & token, const string & lexeme) : value(lexeme), token(token, lexeme, true, "", 0, 0, "") {

    }

    OperatorTableRecord(const Record & token) : value(token.lexeme), token(token) {

    }

    bool operator==(const OperatorTableRecord & other) const {
        return value == other.value;
    }

    bool operator!=(const OperatorTableRecord & other) const {
        return value != other.value;
    }
};

class OperatorPrecedenceParser : public SyntaxAnalyzer {

    enum Productions {
        Expression = 128,
        ExpressionPrime,
        Term,
        TermPrime,
        Factor,
        Epsilon,
        Terminal,
        Error = -2
    };

    /*enum Cells {
        LessDot = '<' + '.',
        DotGreat = '.' + '>',
        Equals = '=',
        ErrorState = 0,
        FileEnd = -1
    };*/

    enum Terminals {
        Addition = 0,
        Subtraction = 0,
        Multiply = 1,
        Divide = 1,
        LeftParen = 2,
        RightParen = 3,
        Identifier = 4,
        EndOfFile = 5,
    };
    list<OperatorTableRecord> productionStack;
    stack<Node *> parseTreeStack;
    static OperatorTableRecord table[6][6];
    static list<pair<OperatorTableRecord, string>> productions;
public:
    OperatorPrecedenceParser(LexicalScanner & lexicalScanner, SymbolTable & symbolTable, ErrorHandler & errorHandler);

    ParseTree * createParseTree();
    bool stackProcess();

    int columnFromToken(Record & token);
    int columnFromRecord(OperatorTableRecord & token);

    int isTerminal(OperatorTableRecord & record) {
        return record.token.lexeme != "";
    }

    string to_string(OperatorTableRecord item) {
        return item.value;
    }

    OperatorTableRecord getTopMostTerminal();
};


#endif //_323_OPERATORPRECEDENCEPARSER_H
