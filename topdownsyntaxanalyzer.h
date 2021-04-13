// top down syntax analyzer
#pragma once
#include "syntaxanalyzer.h"
#include "parsetree.h"
#include "followsets.h"
class TopDownSyntaxAnalyzer : public SyntaxAnalyzer {
    Node * currentNode;
    ParseTree * parseTree;
    FollowSets followSets;

    set<string> statementFollowSet;
    set<string> expressionFollowSet;
    set<string> expressionPrimeFollowSet;
    set<string> termPrimeFollowSet;
public:
    TopDownSyntaxAnalyzer(LexicalScanner & lexicalScanner, SymbolTable & symbolTable, ErrorHandler & errorHandler) 
    : SyntaxAnalyzer(lexicalScanner, symbolTable, errorHandler) {        
        statementFollowSet = { ";", "$", "whileend", "enddo", "endif", "endfor", "endelse"};
        expressionFollowSet = { ")", ">", "<", "<=", ">=", "==", "<>", "do"};
        expressionFollowSet.insert(statementFollowSet.begin(), statementFollowSet.end());
        expressionPrimeFollowSet = expressionFollowSet;
        termPrimeFollowSet = {"+", "-"};
        termPrimeFollowSet.insert(expressionPrimeFollowSet.begin(), expressionPrimeFollowSet.end());
        //set<string> expressionPrimeFollowSet = 
    }

    virtual ParseTree * createParseTree();

    bool isIdentifier(bool check = false);
    bool isIdentifierWithCheck();
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
    bool isConditionalTopDown();
    bool isStatementList();
    bool isMoreStatements();
    
    Node * startNonTerminal(const string & name);
    void finishNonTerminal(Node * parent);
    void cancelNonTerminal(Node * parent);

    bool inFollowSet(set<string> & followSet, const string & token) {
        return followSet.find(token) != followSet.end();
    }
};
