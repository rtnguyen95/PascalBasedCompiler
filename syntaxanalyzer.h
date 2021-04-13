// Syntax Analyzer
#pragma once
#include <list>
#include <vector>
#include "lexicalscanner.h"
#include "parsetree.h"
#include "symboltable.h"

using namespace std;

class SyntaxAnalyzer
{
protected:
    vector<Record> lexemes;
    int currentLexeme; 
    int previousLexeme;
    bool printProduction = true;
    string currentProduction = "";
    string lastError = "";
    LexicalScanner & lexicalScanner;
    ErrorHandler & errorHandler;
    SymbolTable & symbolTable;
public:

    SyntaxAnalyzer(LexicalScanner & lexicalScanner, SymbolTable & symbolTable, ErrorHandler & errorHandler) 
    : lexicalScanner(lexicalScanner), symbolTable(symbolTable), lexemes(), currentLexeme(0), errorHandler(errorHandler) {

    }

    //function to get the next token in the lexemes list
    //returns a Record object of the next lexeme in the list
    Record * getNextToken()
    {
        //check to see if the next token in the list exists. if so, retrieve it and return the Record
        if (currentLexeme < lexemes.size())
        {
            Record & token = lexemes[currentLexeme++];
            return &token;
            //else process the next token
        } else {
            Record token = lexicalScanner.lexer();

            if (token.accepted && token.lexeme.length() > 0) {
                cout << token << endl;
                lexemes.push_back(token);
                Record & token = lexemes[currentLexeme++];
                return &token;
            }
            return NULL; // maybe this should throw an exception instead?
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

    void backup() {
        if (currentLexeme != 0) {
            currentLexeme--;
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
    bool isType(const Record & lexeme);
    bool isId(const Record & lexeme);
    bool isNumber(const Record & lexeme);
    bool isThen(const Record & lexeme);
    bool isWhileEnd(const Record & lexeme);
    bool isDo(const Record & lexeme);
    bool isEndIf(const Record & lexeme);
    bool isEndDo(const Record & lexeme);
    bool isOpenBracket(const Record & lexeme);
    bool isCloseBracket(const Record & lexeme);
    bool isElse(const Record & lexeme);
    bool isEndElse(const Record & lexeme);
    bool isSemiColon(const Record & lexeme);
    bool isOperator(const Record & lexeme);

    vector<Record> & getTokenList() {
        return lexemes;
    }
};
