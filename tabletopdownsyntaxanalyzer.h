#pragma once
#include <stack>
#include <map>
#include "syntaxanalyzer.h"
using namespace::std;

#define RULE(x) {x, "<" #x ">"}

class TableTopDownSyntaxAnalyzer : public SyntaxAnalyzer {

    enum Productions {
        Expression = 128,
        ExpressionPrime,
        Term,
        TermPrime,
        Factor,
        Epsilon,
        Terminal,
        Assignment,
        Declarative,
        Statement,
        StatementList,
        MoreStatements,
        Conditional,
        StatementBlock,
        If,
        ErrorState = -2
    };

    enum Terminals {
        EndofFile = '$',
        Identifier = 'i',
        Number = '1',
        Boolean = '0',
        AssignmentOperator = '=',
        RelativeOperator = '<',
        LeftParenthesis = '(',
        RightParenthesis = ')',
        Addition = '+',
        Subtraction = '-',
        Multiplication = '*',
        Division = '/',
        Equals = '=',
        Type = 't',
        Semicolon = ';',
        IfKeyword = 'f',
        BeginBlock = '{',
        EndBlock = '}'
    };

    map<int, const char *> productions = {
        RULE(Expression),
        RULE(ExpressionPrime),
        RULE(Term),
        RULE(TermPrime),
        RULE(Factor),
        RULE(Terminal),
        RULE(Assignment),
        RULE(Declarative),
        RULE(Statement),
        RULE(StatementList),
        RULE(MoreStatements),
        RULE(Conditional),
        RULE(If),
        RULE(StatementBlock)
    };

    stack<int> productionStack;
    stack<Node *> parseTreeStack;
    map<Productions, map<Terminals, list<int>>> table;
public:
    TableTopDownSyntaxAnalyzer(LexicalScanner & lexicalScanner, SymbolTable & symbolTable, ErrorHandler & errorHandler);
    ParseTree * createParseTree();
    bool stackProcess();

    int columnFromToken(Record & token);

    int isTerminal(int item) {
        return item < 127;
    }

    string to_string(int item) {
        if (isTerminal(item)) {
            return string(1, item);
        } else {
            return productions[item];
        }
    }
};