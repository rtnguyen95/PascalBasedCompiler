#pragma once
#include <stdlib.h>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

//Function to string: accepts a character variable ch and returns a string
inline string toString(char ch)
{
    switch(ch)
    {
        //if ch is a linebreak, returns "\\n"
        case '\n': return "\\n";
        //if ch is a space, returns "[space]"
        case ' ': return "[space]";
        //else returns the character as a string
        default:
            return string(1, ch);
    }
}

//struct to hold information for each token that the FSM processes
//each State object holds an int representing the current state, a char representing the next input, and a bool that returns true if the state is accepted, false if otherwise
struct State
{
    //===Variable Declaration=====
    int state; //represents the current state
    char next_input; //char that holds the next input to be anaylzed by the scanner
    bool accepted; //returns true if the state is accepted, false if otherwise
    
    /*
     Returns a string representation of a state and either the next input or the accepted state if it is the last state in a list of state transitions.
     If the state was 1 and the next_input is w then toString returns (1) --w-->

     if the state was 3 and there is no next input (next_input is 0), then it will use the accepted value and return (3) ----> accepted;

     If a list of states were printed out it might look like this:
     (1) --w--> (2) --h--> (2) --i--> (2) --l--> (2) --e--> (2) --[space]--> (3) ----> accepted;
     */
    string toString()
    {
        //create a string variable result that holds ?? and initialize it to "(state)" where state is the int variable that represents the initial state
        string result = string("(") + to_string(state) + ")";
        
        //if the following character is not blank?
        if (next_input != 0)
        {
            //append "--next_input-->" to result
            result += " --" + ::toString(next_input) + "--> ";
        }
        else{
            //when the end of the input has been reached, appends accepted if the last state is a final state, not accepted if otherwise
            result.append(" ----> ").append(accepted ? "accepted" : "not accepted").append("; ");
        }

        //return the string result to the caller
        return result;
    }
};
//end struct State


//struct Record to hold data for each input (lexeme) that is read
struct Record
{
    string token; //string variable to hold what token the input is (keyword, identifier, etc.)
    string lexeme;  //string variable to hold the raw input
    bool accepted; //boolean variable that returns true if the input has been accepted, false if otherwise
    string errorMessage; //string for outputting an error message if the lexeme is not accepted??
};


inline ostream & operator << (ostream & stream, Record record){
    stream.width(11);
    stream << left << record.token << " = " << record.lexeme;
    if (!record.accepted)
        stream << record.errorMessage;
    return stream;
}


//Class LexicalScanner represents the finite state machine (FSM) that sorts input (lexemes) into tokens
class LexicalScanner
{
private:
    istream & w; //input stream
    
public: 

    list<State> stateTransitions; //list of State objects that holds the state transitions a tokens undergo in the FSM

    
    //?
    LexicalScanner(istream & input) : w(input) {
        
    }

    //Function isFinished accepts no arguments and returns a bool - true if the end of file has been reached, false if otherwise
    //Checks to see if the end of the file has been reached
    bool isFinished()
    {
        return w.peek() == EOF;
    }

protected:
    
    const int q0 = 1; //variable representing the initial state

    const vector<int> FinalStates {3, 5, 7, 9, 10}; //vector of ints holding all the final states of the FSM

    //2D Array of ints representing the FSM transitions.
    //column represents the input character, row represents the state
    const int ntable[10][9] =
    {  //0  1  2  3  4  5  6  7  8
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

        /*
         Column Symbolic Representations
         0      'a'  = alpha
         1      'd'  = digit
         2      '_'  = underscore
         3      '$'  = dollar sign
         4      '.'  = decimal pt
         5      ' '  = space
         6      '!'  = exclamation point - comment maker
         7      '{}' = seperators
         8      '+=' = operators
         */
    };

    const vector<int> backup {3, 5, 7}; //vector of ints holding the backup states

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

    
    //The function char_to_col accepts a character and returns an int representing the column number of the character ch in the FSM table
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

    //Function that checks if the state the FSM is in is a backup state
    //Returns true if the state is a backup state, false if otherwise
    bool isBackupState(int state) {
        return find(backup.begin(), backup.end(), state) != backup.end();
    }

    //Function that checks to see if the FSM is in a final state
    //Returns true if the state is a final state, false if otherwise
    bool inFinal(int state) {
        return find(FinalStates.begin(), FinalStates.end(), state) != FinalStates.end();
    }
    
public:

    //Function to be called for each lexeme to be processed. Returns Record, which holds the token, lexeme, and a boolean flag representing whether the token has been accepted
    Record lexer()
    {
        //Variable Declarations
        int state = q0;     //variable to hold the current state during FSM traversal. initalized to qO, the initial state
        string currentLexeme; //variable to hold the lexeme as it is processed one character at a time
        Record record; //Record object to hold information about the lexeme being processed
        bool reachedFinal = false; //variable that returns true when we get to a state that is a final state, false if otherwise

        //while loop that processes until a final state is reached
        while (!reachedFinal)
        {
            char currChar = w.get(); //variable to hold the current character in the input stream being processed by the FSM. initialized to the first character from the input stream
            
            stateTransitions.push_back({state, currChar}); //add the initial state and the first char to the list of state transitions

            int col = char_to_col(currChar);
            
            if (col != INVALID) {
                state = ntable[state-1][col];
            } else {
                state = 0; // invalid state?
            }

            switch (state) {
                case 2: // inside an identifier or keyword
                    currentLexeme += currChar;
                    break;
                case 3: // end of an identifer or keyword
                    record.lexeme = currentLexeme;
                    if (isKeyword(currentLexeme)) {
                        record.token = "KEYWORD";
                    } else {
                        record.token = "IDENTIFIER";
                    }
                    reachedFinal = true;
                    break;
                case 4: // inside an integer
                    currentLexeme += currChar;
                    break;
                case 5: // at the end of an integer
                    record.token = "INTEGER";
                    record.lexeme = currentLexeme;
                    reachedFinal = true;
                    break;
                case 6: // inside a float
                    currentLexeme += currChar;
                    break;
                case 7: // at the end of a float
                    record.token = "FLOAT";
                    record.lexeme = currentLexeme;
                    reachedFinal = true;
                    break;
                case 8: // in a comment
                    break;
                case 9: // separator
                    record.token = "SEPARATOR";
                    currentLexeme += currChar;
                    record.lexeme = currentLexeme;
                    reachedFinal = true;
                    break;
                case 10:
                    record.token = "OPERATOR";
                    currentLexeme += currChar;
                    record.lexeme = currentLexeme;
                    reachedFinal = true;
                    break;
                case 1:
                    // this will eat whitespace
                    break;
                default:                     
                    cout << "invalid character encountered: "<< currChar <<" Stopping" << endl;
                    reachedFinal = true;
                    break;
            }

            if(isBackupState(state) && !isspace(currChar))
                w.unget();

            if (currChar == EOF)
                break;
        }

        record.accepted = inFinal(state);
        stateTransitions.push_back({state, 0, record.accepted});

        return record;
    }
};
