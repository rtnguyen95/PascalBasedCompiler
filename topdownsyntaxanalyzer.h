// top down syntax analyzer
#pragma once
#include "syntaxanalyzer.h"
#include "parsetree.h"
#include "followsets.h"
class TopDownSyntaxAnalyzer : public SyntaxAnalyzer
{
    Node * currentNode;
    ParseTree * parseTree;
    FollowSets followSets;
    set<string> conditionalFollowSet; //Follow set for <Conditional> 
    set<string> executionFollowSet; //Follow set for block executions 
    set<string> statementFollowSet; //Follow set for <Statement>
    set<string> expressionFollowSet; //Follow set for <Expression>
    set<string> expressionPrimeFollowSet; //Follow set for <Expression Prime>
    set<string> termPrimeFollowSet; //Follow set for <Term Prime>
public:
    TopDownSyntaxAnalyzer(LexicalScanner & lexicalScanner, SymbolTable & symbolTable, ErrorHandler & errorHandler)
    : SyntaxAnalyzer(lexicalScanner, symbolTable, errorHandler) {
        statementFollowSet = { ";", "$"};
        expressionFollowSet = { ")", ">", "<", "<=", ">=", "==", "<>", "do", "{", "then"};
        conditionalFollowSet = {"do", "{", "then"};
        executionFollowSet = {"whileend", "enddo", "endif", "endfor", "endelse", "}"};
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
    bool isElseTopDown();
    bool isBoolValueTopDown();
    Node * startNonTerminal(const string & name);
    void finishNonTerminal(Node * parent);
    void cancelNonTerminal(Node * parent);

    bool inFollowSet(set<string> & followSet, const string & token) {
        return followSet.find(token) != followSet.end();
    }

    bool checkExpressionTypes(Node * node);
    bool checkAssignmentType(Node * node);
    bool checkTypes(Node * node, string & type);
};
