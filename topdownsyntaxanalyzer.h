// top down syntax analyzer
#pragma once
#include "syntaxanalyzer.h"
#include "parsetree.h"

class TopDownSyntaxAnalyzer : public SyntaxAnalyzer {
    Node * currentNode;
    ParseTree * parseTree;
public:
    TopDownSyntaxAnalyzer(LexicalScanner & lexicalScanner, ErrorHandler & errorHandler) : SyntaxAnalyzer(lexicalScanner, errorHandler) {

    }

    virtual ParseTree * createParseTree();

    bool isIdentifier();
    bool isStatement();

    bool isDeclaration();
    bool isTypeTopDown();
    bool isWhileTopDown();
    bool isIfTopDown();
    bool isNumberTopDown();
    bool isQ();
    bool isT();
    bool isR();
    bool isF();
    bool isE();
    bool isAssignment();
    bool conditional();
    bool emptyStatement();

    Node * startNonTerminal(const string & name);
    void finishNonTerminal(Node * parent);
    void cancelNonTerminal(Node * parent);

};
