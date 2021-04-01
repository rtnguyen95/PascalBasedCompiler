// top down syntax analyzer
#pragma once
#include "syntaxanalyzer.h"
#include "parsetree.h"

class TopDownSyntaxAnalyzer : public SyntaxAnalyzer {
    Node * currentNode;
    ParseTree * parseTree;
public:
    TopDownSyntaxAnalyzer(LexicalScanner & lexicalScanner) : SyntaxAnalyzer(lexicalScanner) {

    }

    virtual ParseTree * createParseTree();

    bool isIdentifier();
    bool isStatement();
    
    bool isDeclaration(/*list<Record> & statement*/);


    bool isExpression(list<Record> & statement);
    bool isTerm(list<Record> & statement, list<Record>::iterator & it);
    bool isFactor(list<Record> & statement, list<Record>::iterator & it);


    bool isQ();
    bool isT();
    bool isR();
    bool isF();
    bool isE();
    bool isAssignment();

    Node * startNonTerminal(const string & name);
    void finishNonTerminal(Node * parent);
    void cancelNonTerminal(Node * parent);

};