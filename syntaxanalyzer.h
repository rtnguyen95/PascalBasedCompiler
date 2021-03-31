// Syntax Analyzer
#pragma once
#include <list>
#include <vector>
#include "lexicalscanner.h"
#include "parsetree.h"
using namespace std;

class SyntaxAnalyzer {
protected:
    vector<Record> lexemes;
    //vector<Record>::iterator currentLexeme;
    int currentLexeme;
    int previousLexeme;
    bool printProduction = true;
    LexicalScanner & lexicalScanner;
public:
    //SyntaxAnalyzer(list<Record> & lexemes) : lexemes(lexemes), currentLexeme(lexemes.begin()) {

    //}

    SyntaxAnalyzer(LexicalScanner & lexicalScanner) : lexicalScanner(lexicalScanner), lexemes(), currentLexeme(0) {

    }

    /*list<Record> getNextStatement() {
        list<Record> statement;

        //find the next ; and stop
        while(currentLexeme != lexemes.end()) {

            statement.push_back(*currentLexeme);
            
            if ((currentLexeme++)->lexeme == ";") {
                
                return statement;
            }
        }

        return statement;
    }*/

    /*Record * getNextToken2() {
        if (currentLexeme != lexemes.end()) {
            Record & token = *currentLexeme++;
            return &token;
        } else {
            return NULL;
        }
    }*/

    Record * getNextToken() {
        if (currentLexeme < lexemes.size()) {
            Record & token = lexemes[currentLexeme++];
            return &token;
        } else {
            Record token = lexicalScanner.lexer();
            
            if (!lexicalScanner.isFinished() && token.accepted && token.lexeme.length() > 0) {
                cout << token << endl;
                lexemes.push_back(token);
                Record & token = lexemes[currentLexeme++];
                return &token;
            }
            return NULL;
        }
    }

    /*
      //loop that iterates until we reach the end of the file or we come across an invalid token that cannot reach a final state at the end of processing
  while (!scanner.isFinished() && record.accepted) {
          record = scanner.lexer();       //scan the next string in the input with the lexer and store the result in record
          if (record.lexeme.length())     //the lexeme is blank when there is blank space at the end of a file
          {
            cout << record << endl;  //output the record of the lexeme scanned to the console
            result_code << record << endl; //add the record of the lexeme scanned to the end of recordsList
            records.push_back(record);
          }
  }
    */

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

    bool isType(const Record & lexeme);
    bool isId(const Record & lexeme);
    bool isNumber(const Record & lexeme);

    bool isIdentifier();

    vector<Record> & getTokenList() {
        return lexemes;
    }
};