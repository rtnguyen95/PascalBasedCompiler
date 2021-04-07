// Syntax Analyzer
#pragma once
#include <list>
#include <vector>
#include "lexicalscanner.h"
#include "errorhandler.h"
#include "parsetree.h"
using namespace std;

class SyntaxAnalyzer {
protected:
    vector<Record> lexemes;
    int currentLexeme;
    int previousLexeme;
    bool printProduction = true;
    string currentProduction = "";
    LexicalScanner & lexicalScanner;
    ErrorHandler & errorHandler;
public:

    SyntaxAnalyzer(LexicalScanner & lexicalScanner, ErrorHandler & errorHandler) 
    : lexicalScanner(lexicalScanner), errorHandler(errorHandler), lexemes(), currentLexeme(0) {

    }

    Record * getNextToken() {
        if (currentLexeme < lexemes.size()) {
            Record & token = lexemes[currentLexeme++];
            return &token;
        } else {
            if (lexicalScanner.isFinished()) {
                return NULL; // there are no more tokens
            }
            Record token = lexicalScanner.lexer();

            if (token.accepted && token.lexeme.length() > 0) {
                cout << token << endl;
                lexemes.push_back(token);
                Record & token = lexemes[currentLexeme++];
                return &token;
            }
            return NULL; // there was an error
        }
    }

    void backup() {
        if (currentLexeme != 0) {
            currentLexeme--;
        }
    }

    Record * lookAhead() {
        Record * token = getNextToken();
        backup();
        return token;
    }

    Record * getCurrentToken() {
        if (currentLexeme > 0) {
            return &lexemes[currentLexeme-1];
        } else {
            return nullptr;
        }
    }

    void print(const string & rule) {
        if (printProduction) {
            cout << "     " << rule << endl;
        }
    }

    virtual ParseTree * createParseTree() = 0;
    bool isIf(const Record & lexeme);
    bool isWhile(const Record & lexeme);
    bool isEndWhile(const Record & lexeme);
    bool isType(const Record & lexeme);
    bool isId(const Record & lexeme);
    bool isNumber(const Record & lexeme);
    bool isOperator(const Record & token, char op);
    bool isOperator(const Record & token, const string & op);
    bool isSeparator(const Record & token, char op);
    bool isSemiColon(const Record & token) {
        return isSeparator(token, ';');
    }
    bool isConditionalOperator(const Record & token);
    vector<Record> & getTokenList() {
        return lexemes;
    }
};
