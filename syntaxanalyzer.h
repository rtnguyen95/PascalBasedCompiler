// Syntax Analyzer
#pragma once
#include <list>
#include <vector>
#include "lexicalscanner.h"
#include "parsetree.h"
#include "symboltable.h"
#include "instruction.h"

using namespace std;

int instr_address = 0; //global variable for intermediate code generation

class SyntaxAnalyzer
{
protected:
    vector<Record> lexemes;
    int currentLexeme;
    int previousLexeme;
    Record currentToken;
    bool printProduction = true;
    string currentProduction = "";
    string lastError = "";
    LexicalScanner & lexicalScanner;
    ErrorHandler & errorHandler;
    SymbolTable & symbolTable;
    Node * currentNode;
    ParseTree * parseTree;
    vector <Instruction> instr_table;
    
public:

    SyntaxAnalyzer(LexicalScanner & lexicalScanner, SymbolTable & symbolTable, ErrorHandler & errorHandler)
    : lexicalScanner(lexicalScanner), symbolTable(symbolTable), lexemes(), currentLexeme(0), errorHandler(errorHandler) {

    }

    //function to get the next token in the lexemes list. This function is used to support backtracking. When the syntax analyzer attempts to find a rule by trying all the rules on a token one by one, aka a brute force approach (as opposed to a predictive approach), backtracking is needed when a rule fails and a token needs to be tested from the beginning for the next rule.
    //returns a pointer to the Record object of the next lexeme in the list
    Record * getNextToken()
    {
        //check to see if there is a token after currentLexeme. If there is, return a pointer to it
        if (currentLexeme < lexemes.size())
        {
            currentToken = lexemes[currentLexeme++];
            return &currentToken;
        //else getNextToken is being called for the first time, and the lexemes list is empty. it will call lexicalScanner.lexer() and add the token to the list and return a pointer to that token
        } else {
            Record token = lexicalScanner.lexer();
            cout << token << endl;
            lexemes.push_back(token);
            currentToken = lexemes[currentLexeme++];
            return &currentToken;
        }
    }

    //function to peek ahead at the next token in the list
    //Returns the next token in the list
    Record * lookAhead()
    {
        Record * token = getNextToken();
        backup(); //function to backup variables so that the peek does not affect processing
        return token;
    }

    //function to get the current token being processed by the syntax analyzer
    //Returns a pointer to the token
    Record * getCurrentToken() {
        if (currentLexeme >= 0) {
            return &lexemes[currentLexeme > 0 ? currentLexeme-1 : 0];
        } else {
            return nullptr;
        }
    }

    //function to backup currentLexeme
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
    
    void gen_instr(string op, string oprnd)
    {
        Instruction instruction = Instruction(op, oprnd);
        instr_table[instr_address] = instruction;
        instr_address++;
    }

    //Functions that determine the lexemes and tokens
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
    bool isRelativeOperator(const Record & lexeme);
    bool isError(const Record & lexeme);
    bool isBoolValue(const Record & lexeme);

    const vector<string> relativeOperators = {"<", ">", "<=", ">=", "<=", "==", "<>"};

    bool isPlus(const Record & lexeme) {
        return lexeme.token == "OPERATOR" && lexeme.lexeme == "+";
    }

    bool isMinus(const Record & lexeme) {
        return lexeme.token == "OPERATOR" && lexeme.lexeme == "-";
    }

    bool isMultiply(const Record & lexeme) {
        return lexeme.token == "OPERATOR" && lexeme.lexeme == "*";
    }

    bool isDivideBy(const Record & lexeme) {
        return lexeme.token == "OPERATOR" && lexeme.lexeme == "/";
    }

    bool isLeftParen(const Record & lexeme) {
        return lexeme.token == "SEPARATOR" && lexeme.lexeme == "(";
    }

    bool isRightParan(const Record & lexeme) {
        return lexeme.token == "SEPARATOR" && lexeme.lexeme == ")";
    }

    bool isEOF(const Record & lexeme) {
        return lexeme.token == "EOF" && lexeme.lexeme == "$";
    }

    bool isEquals(const Record & lexeme) {
        return lexeme.token == "OPERATOR" && lexeme.lexeme == "=";
    }

    bool isBeginBlock(const Record & lexeme) {
        return lexeme.token == "SEPARATOR" && lexeme.lexeme == "{";
    }

    bool isEndBlock(const Record & lexeme) {
        return lexeme.token == "SEPARATOR" && lexeme.lexeme == "}";
    }

    //function to return the vector of lexemes
    vector<Record> & getTokenList() {
        return lexemes;
    }
};
