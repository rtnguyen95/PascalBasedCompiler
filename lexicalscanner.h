#pragma once
#include <stdlib.h>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include "errorhandler.h"
#include "record.h"

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

// struct to hold information for each token that the FSM processes
// each State object holds an int representing the current state,
// a char representing the next input, and a bool that returns true if the state is accepted, false if otherwise
struct State
{
    //===Variable Declaration=====
    int state; //represents the current state
    char next_input; //char that holds the next input to be anaylzed by the scanner
    bool accepted; //returns true if the state is accepted, false if otherwise

    /*
     toString() returns a string representation of the state transitions an input undergoes.
     
     Each time a state transition is completed the input and its accompanying state appends to a string. When the input processing is complete the string "accepted" / "not accepted" is appended to denote if the FSM ended in a final state.
     
     Examples:
     If the state was 1 and the next_input is w then toString returns (1) --w-->

     if the state was 3 and there is no next input (next_input is 0), then it will use the accepted value and return (3) ----> accepted;

     toString() for w= while
     (1) --w--> (2) --h--> (2) --i--> (2) --l--> (2) --e--> (2) --[space]--> (3) ----> accepted;
     */
    string toString()
    {
        //create a string variable result that holds inforation on this state transition.
        // and initialize it to "(state)" where state is the int variable that represents the state
        string result = string("(") + to_string(state) + ")";

        //if the following character is not blank, which signals the final state
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





//Class LexicalScanner represents the finite state machine (FSM) that sorts input (lexemes) into tokens
class LexicalScanner
{
private:
    istream & w; //input stream
    const string & filename;
    ErrorHandler & errorHandler;
    Record lastLexeme;
public:

    list<State> stateTransitions; //list of State objects that holds the state transitions a tokens undergo in the FSM


    //
    // This constructor takes a inputstream that will be used to scan for tokens
    //
    LexicalScanner(istream & input, const string & filename, ErrorHandler & errorHandler): 
        w(input), filename(filename), errorHandler(errorHandler) {

    }

    //Function isFinished accepts no arguments and returns a bool
    // returns true if the end of file has been reached, false if otherwise
    //Checks to see if the end of the file has been reached
    bool isFinished()
    {
        return lastLexeme.token == "EOF";
    }

protected:

    const int q0 = 1; //variable representing the initial state

    const vector<int> FinalStates {3, 5, 7, 10, 11, 14, 15, 16, 17, 18, 19}; //vector of ints holding all the final states of the FSM

    //2D Array of ints representing the FSM transitions.
    //column represents the input character, row represents the state 
    const int ntable[19][14] = {
       //a,  d,  _,  $,  .,   ,  !,  {}, +*, =,  <,  >,  /\, EOF
        {2,  4,  13, 13, 9,  1,  8,  10, 12, 12, 18, 19, 13, 17},  // 1  starting state
        {2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3},   // 2  in identifier
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},   // 3  end identifier (final state)
        {5,  4,  5,  5,  6,  5,  5,  5,  5,  5,  5,  5,  5,  5},   // 4  in integer
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},   // 5  end integer (final state)
        {7,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7},   // 6  in float
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},   // 7  end float (final state)
        {8,  8,  8,  8,  8,  8,  1,  8,  8,  8,  8,  8,  8,  14},  // 8  in comment
        {11, 6,  11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11},  // 9  Found a . (decimal point or separator)
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},   //10 separator, no backup * see note
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},   //11 separator, backup
        {16, 16, 16, 16, 16, 16, 16, 16, 16, 15, 13, 13, 16, 16},   // 12 found operator (single operators only, no compounds) 
        {13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 13, 14},           // 13 Error state
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},             // 14 error state end
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},   // 15 End Equals State (final)
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  // 16 End operator state with backup
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  // 17 EndOfFile state without backup
        {16, 16, 16, 16, 16, 16, 16, 16, 16, 15, 16, 19, 16, 16},  // 18 < operator state with backup
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},  // 19 End <> state without backup
    };

        /*
         Column Symbolic Representations
         0      'a'  = alpha
         1      'd'  = digit
         2      '_'  = underscore
         3      '$'  = dollar sign
         4      '.'  = decimal pt
         5      ' '  = space
         6      '!'  = exclamation point - comment maker
         7      '{}' = separators
         8      '+=' = operators
         9      other=  any other character not part of any other rule such as (#, \, “)
         */
    
    
    /*
     Note on backups:
     Backups are used when different tokens are not separated by a space. an input character is read that is not part of the current lexeme triggers the current lexeme to be complete. The input must go back one character to reread the start of the next lexeme.
     
     Example:
     consider that this is the string being read: "int x;"
    1. int will be read and then the space triggers that the token has ended.
    2. x will be read and the ; triggers that the token has ended, but the lexer needs to backup by 1 character so that ; will be read the next time the lexer is called.
    3. ; is read and then the operations are complete.
     */

    const vector<int> backup {3, 5, 7, 11, 14, 16}; //vector of ints holding the backup states

    //enumeration for the columns of the state table (starts at index 0)
    enum {
        ALPHA,              //col 0
        DIGIT,              //col 1
        UNDERSCORE,         //col 2
        DOLLAR_SIGN,        //col 3
        DECIMAL_PT,         //col 4
        WHITE_SPACE,        //col 5
        COMMENT_MARKER,     //col 6
        SEPARATOR,          //col 7
        OPERATOR,           //col 8
        EQUALS,             //col 9
        LESS_THAN,          //col 10
        GREATER_THAN,       //col 11
        OTHER,              //col 12
        ENDOFFILE           //col 13
    };


    //The function char_to_col accepts a character and returns an int representing the column number of the character ch in the FSM table
    int char_to_col(char ch)
    {
        //check to see if the character is a digit and if true return the column number for digit
        if (isdigit(ch))
            return DIGIT;

        //check to see if the character is a . and if true return the column number for decimal point
        if (ch == '.')
            return DECIMAL_PT;

        //checks to see if the character is a space OR if the end of the file has been reached and if true returns the column number for space
        if (isspace(ch))
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

        if (ch == '=')
            return EQUALS;

        //checks to see if the character is in the list of seperators recognized by the analyzer and if true return the column number for seperator
        if (isSeparator(ch))
            return SEPARATOR;

        if (ch == '<')
            return LESS_THAN;
        
        if (ch == '>')
            return GREATER_THAN;

        //checks to see if the character is in the list of operators recognized by the analyzer and if true return the column number for operator
        if (isOperator(ch))
            return OPERATOR;

        if (ch == EOF) {
            return ENDOFFILE;
        }

        //if the character does not match any of the cases above the character is invalid, and the function returns an int for an invalid character
        return OTHER;
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
        '*', '+', '-', '/', '<', '>', '%'
    };

    //function that accepts a string and checks to see if the string is in the list of keywords that the analyzer recognizes
    //Returns a boolean value - true if the string is a keyword, false if otherwise
    bool isKeyword(const string & str) {
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
    
    //==Function definitions for handling each state ==

    bool processStartState(string & currentLexeme, char currChar, Record & record);

    bool processIdentifierOrKeywordState(string & currentLexeme, char currChar, Record & record);
    bool processEndIdentiferOrKeywordState(string & currentLexeme, char currChar, Record & record);

    bool processIntegerState(string & currentLexeme, char currChar, Record & record);
    bool processEndIntegerState(string & currentLexeme, char currChar, Record & record);

    bool processFloatState(string & currentLexeme, char currChar, Record & record);
    bool processEndFloatState(string & currentLexeme, char currChar, Record & record);

    bool processCommentState(string & currentLexeme, char currChar, Record & record);

    bool processDecimalPointState(string & currentLexeme, char currChar, Record & record);

    bool processSeparatorState(string & currentLexeme, char currChar, Record & record);
    
    bool processEndSeparatorState(string & currentLexeme, char currChar, Record & record);

    bool processOperatorState(string & currentLexeme, char currChar, Record & record);
    bool processEndOperatorState(string & currentLexeme, char currChar, Record & record);
    bool processEndEqualsState(string & currentLexeme, char currChar, Record & record);

    bool processErrorState(string & currentLexeme, char currChar, Record & record);
    bool processEndErrorState(string & currentLexeme, char currChar, Record & record);

    bool processEndOfFileState(string & currentLexeme, char currChar, Record & record);
    
    bool processLessThanState(string & currentLexeme, char currChar, Record & record);
    bool processEndInequalityState(string & currentLexeme, char currChar, Record & record);
    
    int line = 1;
    int linePosition = 1;
public:

    //Function to be called for each lexeme to be processed. Returns Record, which holds the token, lexeme, and a boolean flag representing whether the token has been accepted
    Record lexer()
    {

        //Variable Declarations
        int state = q0;     //variable to hold the current state during FSM traversal. initalized to qO, the initial state
        string currentLexeme; //variable to hold the lexeme as it is processed one character at a time
        Record record; //Record object to hold information about the lexeme being processed
        bool reachedFinal = false; //variable that returns true when we get to a state that is a final state, false if otherwise

        bool hasError = false;

        //while loop that processes until a final state is reached
        while (!reachedFinal)
        {
            char currChar = w.get(); //create a variable to hold the current character in the input stream being processed by the FSM and initialize to the first character from the input stream

            stateTransitions.push_back({state, currChar}); //add the current state and the current char to the list of state transitions

            int col = char_to_col(currChar); //create a variable to hold the column corresponding to the current character and initialize to the column of the first character

            //get the next state from the ntable
            state = ntable[state-1][col]; //the index of the row is state-1, since the initial state has been represented as 1 instead of 0 in the program
            
            // else state = state
            //switch statement that executes based on which state the FSM has entered
            switch (state)
            {
                //eats whitespace
                case 1:
                    reachedFinal = processStartState(currentLexeme, currChar, record);
                    break;
                // inside an identifier or keyword
                case 2:
                    reachedFinal = processIdentifierOrKeywordState(currentLexeme, currChar, record);
                    break;
                    
                // end of an identifer or keyword - Final State
                case 3:
                    reachedFinal = processEndIdentiferOrKeywordState(currentLexeme, currChar, record);
                    //reachedFinal = true; //set the boolean flag for reaching a final state as true
                    break;

                // inside an integer
                case 4:
                    reachedFinal = processIntegerState(currentLexeme, currChar, record);
                    break;

                // at the end of an integer - Final State
                case 5:
                    reachedFinal = processEndIntegerState(currentLexeme, currChar, record);
                     break;

                // inside a float
                case 6:
                    reachedFinal = processFloatState(currentLexeme, currChar, record);
                    break;

                // at the end of a float - Final State
                case 7:
                    reachedFinal = processEndFloatState(currentLexeme, currChar, record);

                    break;

                //in a comment - no processing
                case 8:
                    reachedFinal = processCommentState(currentLexeme, currChar, record);
                    break;

                //found .
                case 9:
                    // found a decimal point, the next character deterimines
                    // if it is a separator or floating point number
                    reachedFinal = processDecimalPointState(currentLexeme, currChar, record);
                    break;

                //found separator - Final state
                case 10: // separator
                    reachedFinal = processSeparatorState(currentLexeme, currChar, record);
                    break;

                // end separator (such as .) - Final State
                case 11: // separator was found in the previous state
                    reachedFinal = processEndSeparatorState(currentLexeme, currChar, record);
                    break;

                //found operator - final state
                case 12: // operators
                    reachedFinal = processOperatorState(currentLexeme, currChar, record);
                    break;

                case 13: 
                    reachedFinal = processErrorState(currentLexeme, currChar, record);
                    break;
                case 14: 
                    reachedFinal = processEndErrorState(currentLexeme, currChar, record);
                    break;
                case 15:
                    reachedFinal = processEndEqualsState(currentLexeme, currChar, record);
                    break;
                case 16:
                    reachedFinal = processEndOperatorState(currentLexeme, currChar, record);
                    break;
                case 17:
                    reachedFinal = processEndOfFileState(currentLexeme, currChar, record);
                    break;
                case 18:
                    reachedFinal = processLessThanState(currentLexeme, currChar, record);
                    break;
                case 19:
                    reachedFinal = processEndInequalityState(currentLexeme, currChar, record);
                    break;                    
                //invalid character - any character not recognized by the FSM
                //this should not occur
                default:
                    //output to the user that an invalid character has been encountered and stop the processing
                    cout << "Invalid character encountered: "<< currChar <<" Stopping" << endl;
                    reachedFinal = true;
                    break;
            }

            // If the current state requires that the current input be processed again for the next token
            // then rewind the inputstream by one character
            if(isBackupState(state)) {
                w.unget();
            } else {
                linePosition++;
            }

            lastLexeme = record;
            // if the end of file has been reached, then stop the while loop
            //if (currChar == EOF) {

            //    break; // trigger the end of the while loop.  Setting reachedFinal = true is not sufficent
            //}
        }

        //check to see if the state is in a final state and write the result to the record
        record.accepted = inFinal(state);
        //add the state and whether it was accepted to the list of state transitions
        stateTransitions.push_back({state, 0, record.accepted});

        //return the record to the caller
        return record;
    }
};
