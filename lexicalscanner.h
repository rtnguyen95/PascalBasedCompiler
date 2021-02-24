#pragma once
#include <stdlib.h>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

//Function Description
inline string toString(char ch)
{
    switch(ch)
    {
        case '\n': return "\\n";
        case ' ': return "[space]";
        default: 
            return string(1, ch);
    }
}

//struct description
struct State
{
    int state;
    char next_input;
    bool accepted;

    string toString()
    {
        string result = string("(") + to_string(state) + ")";
        if (next_input != 0)
        {
            result += " --" + ::toString(next_input) + "--> ";
        }
        else
        {
            result.append(" ----> ").append(accepted ? "accepted" : "not accepted").append("; ");
        }

        return result;
    }
};

//Description
struct Record
{
    string token;
    string lexeme;
    bool accepted;
    string errorMessage;
};

//Description
inline ostream & operator << (ostream & stream, Record record)
{
    stream.width(11);
    stream << left << record.token << " = " << record.lexeme;
    if (!record.accepted)
        stream << record.errorMessage;
    return stream;
}

class LexicalScanner
{
private:
    istream & w;
    
public: 

    list<State> stateTransitions;

    LexicalScanner(istream & input) : w(input) {
        
    }

    bool isFinished() {
        return w.peek() == EOF;
    }

protected:
    int q0 = 1;                 // initial state

    vector<int> F {3, 5, 7, 9, 10}; // final states

    const int ntable[10][9] = {
       //a, d, _, $, .,  , !, {}, +=
        {2, 4, 1, 1, 9, 1, 8, 9, 10},   // 1 starting state
        {2, 2, 2, 2, 3, 3, 3, 3, 3},    // 2 in identifier
        {1, 1, 1, 1, 1, 1, 1, 1, 1},    // 3 end identifier (final state)
        {5, 4, 5, 5, 6, 5, 5, 5, 5},    // 4 in integer
        {1, 1, 1, 1, 1, 1, 1, 1, 1},    // 5 end integer (final state)
        {7, 6, 7, 7, 7, 7, 7, 7, 7},    // 6 in float
        {1, 1, 1, 1, 1, 1, 1, 1, 1},    // 7 end float (final state)
        {8, 8, 8, 8, 8, 8, 1, 8, 8},    // 8 in comment
        {1, 1, 1, 1, 1, 1, 1, 1, 1},    // 9 separator (final state)
        {1, 1, 1, 1, 1, 1, 1, 1, 1},     //10 end operator, single operators only (final state)

    };

    vector<int> backup {3, 5, 7};

    //enumeration for the columns of the state table (starts at index 0)
    enum
    {
        ALPHA,              //col 0
        DIGIT,              //col 1
        UNDERSCORE,         //col 2
        DOLLAR_SIGN,        //col 3
        DECIMAL_PT,         //col 4
        WHITE_SPACE,        //col 5
        COMMENT_MARKER,     //col 6
        SEPARATOR,          //col 7
        OPERATOR,           //col 8
        INVALID = 100
    };

    //The function char_to_col accepts a character and returns an int representing the column number of the character ch in the DFSM table
    int char_to_col(char ch)
    {
        //check to see if the character is a digit and if true return the column number for digit
        if (isdigit(ch))
            return DIGIT;
        //check to see if the character is a . and if true return the column number for decimal
        if (ch == '.')
            return DECIMAL_PT;
        //checks to see if the character is a space OR if the end of the file has been reached and if true returns the column number for space
        if (isspace(ch) || ch == EOF)
            return WHITE_SPACE;
        //checks to see if the character is an alphabetical character and if true returns the column number for alphabet
        if (isalpha(ch))
            return ALPHA;
        //checks to see if the character is an underscore and if true returns the column number for underscore
        if (ch == '_')
            return UNDERSCORE;
        //checks to see if the character is a dollar sign and if true returns the column number for dollar sign
        if (ch == '$')
            return DOLLAR_SIGN;
        //checks to see if the character is an explanation point (exclamation point denotes the beginning and end of a comment) and if true returns the column number for comment maker
        if (ch == '!')
            return COMMENT_MARKER;
        //checks to see if the character is in the list of seperators recognized by the analyzer and if true return the column number for seperator
        if (isSeparator(ch))
            return SEPARATOR;
        //checks to see if the character is in the list of operators recognized by the analyzer and if true return the column number for operator
        if (isOperator(ch))
            return OPERATOR;
        //if the character does not match any of the cases above the character is invalid, and the function returns an int for an invalid character
        return INVALID;
    }

    //a vector holding all the keywords the analyzer recognizes
    const vector<string> keywords ={
        "int", "float", "bool", 
        "True", "False",
        "if", "else", "then", "endif", "endelse", 
        "while", "whileend", 
        "do", "enddo", 
        "for", "endfor",
        "STDinput", "STDoutput",
        "and", "or", "not"
    };

    //a vector holding all the separators that the analyzer recognizes
    const vector<char> separators ={
        '(', ')', '{', '}', '[', ']', ',', '.', ':', ';'
    };

    //a vector holding all the operators that the analyzer recognizes
    const vector<char> operators ={
        '*', '+', '-', '=', '/', '<', '>', '%'
    };

    //function that accepts a string and checks to see if the string is in the list of keywords that the analyzer recognizes
    //Returns a boolean value - true if the string is a keyword, false if otherwise
    bool isKeyword(string & str) {
        return find(keywords.begin(), keywords.end(), str) != keywords.end();
    }

    //Function that accepts a character and checks to see if the character is in the list of separators that the analyzer recognizes
    //Returns a boolean value - true if the character is a separator, false if otherwise
    bool isSeparator(char ch) {
        return find(separators.begin(), separators.end(), ch) != separators.end();
    }

    //Function that accepts a character and checks to see if the character is in the list of operators that the analyzer recognizes
    //Returns a boolean value - true if the character is an operator, false if otherwise
    bool isOperator(char ch) {
        return find(operators.begin(), operators.end(), ch) != operators.end();
    }

    //Function description
    bool isBackupState(int state) {
        return find(backup.begin(), backup.end(), state) != backup.end();
    }

    //Function description
    bool inF(int state) {
        return find(F.begin(), F.end(), state) != F.end();
    }
    
public:

    Record lexer() {
        int state = q0;
        string currentLexeme;
        Record record;
        bool reachedEnd = false;

        while (!reachedEnd) {
            char ch = w.get();
            stateTransitions.push_back({state, ch});

            int col = char_to_col(ch);
            if (col != INVALID) {
                state = ntable[state-1][col];
            } else {
                state = 0; // invalid state?
            }

            switch (state) {
                case 2: // inside an identifier or keyword
                    currentLexeme += ch;
                    break;
                case 3: // end of an identifer or keyword
                    record.lexeme = currentLexeme;
                    if (isKeyword(currentLexeme)) {
                        record.token = "KEYWORD";
                    } else {
                        record.token = "IDENTIFIER";
                    }
                    reachedEnd = true;
                    break;
                case 4: // inside an integer
                    currentLexeme += ch;
                    break;
                case 5: // at the end of an integer
                    record.token = "INTEGER";
                    record.lexeme = currentLexeme;
                    reachedEnd = true;
                    break;
                case 6: // inside a float
                    currentLexeme += ch;
                    break;
                case 7: // at the end of a float
                    record.token = "FLOAT";
                    record.lexeme = currentLexeme;
                    reachedEnd = true;
                    break;
                case 8: // in a comment
                    break;
                case 9: // separator
                    record.token = "SEPARATOR";
                    currentLexeme += ch;
                    record.lexeme = currentLexeme;
                    reachedEnd = true;
                    break;
                case 10:
                    record.token = "OPERATOR";
                    currentLexeme += ch;
                    record.lexeme = currentLexeme;
                    reachedEnd = true;
                    break;
                case 1:
                    // this will eat whitespace
                    break;
                default:                     
                    cout << "invalid character encountered: "<< ch <<" Stopping" << endl; 
                    reachedEnd = true;
                    break;
            }

            if(isBackupState(state) && !isspace(ch))
                w.unget();

            if (ch == EOF)
                break;
        }

        record.accepted = inF(state);
        stateTransitions.push_back({state, 0, record.accepted});

        return record;
    }
};
