#include "lexicalscanner.h"
#include "errorhandler.h"
/**
 * processes the start state state. 
 * 
 * No processing is performed for the start state.  Besides starting the lexer, 
 * this state will also eat white space and unrecognized characters.
 * 
 * @param currentLexeme is normally blank
 * @param currChar contains the current character
 * @param record will be not be set
 * 
 * @return always returns false, since this is not a final state. 
 * 
 */
bool LexicalScanner::processStartState(string & currentLexeme, char currChar, Record & recordr) {
    // do nothing on the start state
    if (currChar == '\n') {
        line++;
        linePosition = 0; // reset the character position in the line
    }
    return false; // this is not a final state
}

/**
 * processes the in identifier or keyword state. 
 * 
 * adds the current character to the current lexeme
 *  
 * @param currentLexeme contains the currentlexeme (not used)
 * @param currChar contains the current character
 * @param record will be not be set
 * 
 * @return always returns false, since this is not a final state. 
 * 
 */
bool LexicalScanner::processIdentifierOrKeywordState(string & currentLexeme, char currChar, Record & recordr) {
    currentLexeme += currChar;
    return false; // this is not a final state
}

/**
 * processes the end identifier/keyword state.
 * 
 * Sets the record to the appropriate token and lexeme. Checks to see if the current lexeme
 * matches an item from the list of keywords to determine if it is a keyword.
 * 
 * @param currentLexeme contains the current lexeme
 * @param currChar contains the current character (not used)
 * @param record will be set for this token
 * 
 * @return always returns true, since this is a final state
 */
bool LexicalScanner::processEndIdentiferOrKeywordState(string & currentLexeme, char currChar, Record & record) {
    record.set("", currentLexeme, true, filename, line, linePosition);

    //check to see if the lexeme is a keyword - save the token as keyword if true, identifier if otherwise
    if (isKeyword(currentLexeme))
    {
        record.token = "KEYWORD";
    }
    else
    {
        record.token = "IDENTIFIER";
    }    
    return true; // this is a final state
}

/**
 * processes the in integer state. 
 * 
 * adds the current character to the current lexeme
 *  
 * @param currentLexeme contains the currentlexeme
 * @param currChar contains the current character
 * @param record will be not be set
 * 
 * @return always returns false, since this is not a final state. 
 * 
 */
bool LexicalScanner::processIntegerState(string & currentLexeme, char currChar, Record & record) {
    currentLexeme += currChar;    
    return false; // this is not a final state
}

/**
 * processes the end integer state
 * 
 * set the record to the appropriate token and lexeme
 * 
 * @param currentLexeme is the current lexeme
 * @param currChar contains the current character which ended the integer (not used)
 * @param record will be set for this token
 * 
 * @return always returns true, since this is a final state
 */
bool LexicalScanner::processEndIntegerState(string & currentLexeme, char currChar, Record & record) {
    record.set("INTEGER", currentLexeme, true, filename, line, linePosition);
    return true; // this is a final state
}

/**
 * processes the in float state. 
 * 
 * adds the current character to the current lexeme
 *  
 * @param currentLexeme contains the currentlexeme
 * @param currChar contains the current character
 * @param record will be not be set
 * 
 * @return always returns false, since this is not a final state. 
 * 
 */
bool LexicalScanner::processFloatState(string & currentLexeme, char currChar, Record & record) {
    currentLexeme += currChar;
    return false; // this is not a final state
}

/**
 * processes the end float state
 * 
 * set the record to the appropriate token and lexeme
 * 
 * @param currentLexeme is the current lexeme
 * @param currChar contains the current character which ended the float (not used)
 * @param record will be set for this token
 * 
 * @return always returns true, since this is a final state
 */
bool LexicalScanner::processEndFloatState(string & currentLexeme, char currChar, Record & record) {
    record.set("FLOAT", currentLexeme, true, filename, line, linePosition);
    return true; // this is a final state
}

/**
 * processes the comment state state. 
 * 
 * No processing is performed for comments
 * 
 * @param currentLexeme is normally blank
 * @param currChar contains the current character
 * @param record will be not be set
 * 
 * @return always returns false, since this is not a final state. 
 * 
 */
bool LexicalScanner::processCommentState(string & currentLexeme, char currChar, Record & record) {
    // do not do any process for comments
    return false; // this is not a final state
}

/**
 * processes the decimal point (.) state
 * 
 * starts a new lexeme that begins with the .
 * 
 * @param currentLexeme is normally blank
 * @param currChar contains the .
 * @param record will be not be set
 * 
 * @return always returns false, since this is not a final state. 
 *
 * The next state will determine if the . is a separator or the begining of a floating point number.
 * 
 */
bool LexicalScanner::processDecimalPointState(string & currentLexeme, char currChar, Record & record) {
    currentLexeme += currChar;
    return false; // this is not a final state
}

/**
 * processes the separator state
 * 
 * set the record to the appropriate token and lexeme
 * 
 * @param currentLexeme is normally blank and is not used
 * @param currChar contains the separator
 * @param record will be set for this token
 * 
 * @return always returns true, since this is a final state
 */
bool LexicalScanner::processSeparatorState(string & currentLexeme, char currChar, Record & record) {
    //set the final data in the record and set the boolean flag for reaching a final state as true
    record.set("SEPARATOR", currentLexeme + string(1, currChar), true, filename, line, linePosition);
    return true; // this is a final state
}

/**
 * processes the end separator state
 * 
 * set the record to the appropriate token and lexeme
 * 
 * @param currentLexeme is contains the separator
 * @param currChar contains the character after the separator (not sued)
 * @param record will be set for the current token/lexeme
 * 
 * @return always returns true, since this is a final state
 */
bool LexicalScanner::processEndSeparatorState(string & currentLexeme, char currChar, Record & record) {
    //set the final data in the record and set the boolean flag for reaching a final state as true
    record.set("SEPARATOR", currentLexeme, true, filename, line, linePosition);
    return true; // this is a final state
}

/**
 * processes the operator state
 * 
 * sets the record to the appropriate token and lexeme
 * 
 * @param currentLexeme is normally blank and is not used
 * @param currChar contains the operator
 * @param record will be set for this token
 * 
 * @return always returns true, since this is a final state
 * 
 */
bool LexicalScanner::processOperatorState(string & currentLexeme, char currChar, Record & record) {
    //set the final data in the record and set the boolean flag for reaching a final state as true
    //record.set("OPERATOR", currentLexeme + string(1, currChar), true, filename, line, linePosition);
    currentLexeme = currChar;
    return false; // this is a final state
}

/**
 * processes the end operator state
 * 
 * sets the record to the appropriate token and lexeme
 * 
 * @param currentLexeme is normally blank and is not used
 * @param currChar contains the operator
 * @param record will be set for this token
 * 
 * @return always returns true, since this is a final state
 * 
 */
bool LexicalScanner::processEndOperatorState(string & currentLexeme, char currChar, Record & record) {
    //set the final data in the record and set the boolean flag for reaching a final state as true
    record.set("OPERATOR", currentLexeme, true, filename, line, linePosition);
    return true; // this is a final state
}

/**
 * processes the end operator state
 * 
 * sets the record to the appropriate token and lexeme
 * 
 * @param currentLexeme is normally blank and is not used
 * @param currChar contains the operator
 * @param record will be set for this token
 * 
 * @return always returns true, since this is a final state
 * 
 */
bool LexicalScanner::processEndEqualsState(string & currentLexeme, char currChar, Record & record) {
    //set the final data in the record and set the boolean flag for reaching a final state as true
    currentLexeme += currChar;
    record.set("OPERATOR", currentLexeme, true, filename, line, linePosition);
    return true; // this is a final state
}

/**
 * processes the error state
 * 
 * sets the record to the appropriate token and lexeme
 * 
 * @param currentLexeme contains the current lexeme
 * @param currChar contains the current character
 * @param record will be set for this token
 * 
 * @return always returns true, since this is a final state
 * 
 */
bool LexicalScanner::processErrorState(string & currentLexeme, char currChar, Record & record) {
    currentLexeme += currChar;
    return false; // this is a final state
}

/**
 * processes the end error state
 * 
 * sets the record to the appropriate token and lexeme
 * 
 * @param currentLexeme is the invalid lexeme
 * @param currChar contains the current character, but is not used
 * @param record will be set for this invalid token
 * 
 * @return always returns true, since this is a final state
 * 
 */
bool LexicalScanner::processEndErrorState(string & currentLexeme, char currChar, Record & record) {
    //set the final data in the record and set the boolean flag for reaching a final state as true
    string message;
    if (currentLexeme[0] == '$' || currentLexeme[0] == '_') {
        message.append(string(1, currentLexeme[0]).append(" is only allowed inside of an identifier: ").append(currentLexeme));
    } else {
        message.append(string(1, currentLexeme[0])).append (" is not an allowed character: ").append(currentLexeme);
    }

    record.set("ERROR", currentLexeme, false, filename, line, linePosition, message);
    error currentError = {filename, line, linePosition - (int)currentLexeme.length(), message, lexical_error};
    errorHandler.addError(currentError);
    cout << currentError.toString() << endl;
    return true; // this is a final state
}

/**
 * processes the end of file state
 * 
 * sets the record to the appropriate token and lexeme
 * 
 * @param currentLexeme is normally blank and is not used
 * @param currChar contains the operator
 * @param record will be set for this token
 * 
 * @return always returns true, since this is a final state
 * 
 */
bool LexicalScanner::processEndOfFileState(string & currentLexeme, char currChar, Record & record) {
    //set the final data in the record and set the boolean flag for reaching a final state as true
    record.set("EOF", "$", true, filename, line, linePosition);
    return true; // this is a final state
}