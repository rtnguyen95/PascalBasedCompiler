#include "lexicalscanner.h"

/**
 * processes the in identifier/keyword state
 * 
 * add the current character to the current lexeme
 * 
 */
void LexicalScanner::processIdentifierState(string & currentLexeme, char currChar) {
    currentLexeme += currChar;
}

/**
 * processes the end identifier/keyword state
 * 
 * set the record to the appropriate token and lexeme
 */
void LexicalScanner::processEndIdentiferState(const string & currentLexeme, Record & record) {
    record.lexeme = currentLexeme; //save the final lexeme string in record
    //check to see if the lexeme is a keyword - save the token as keyword if true, identifier if otherwise
    if (isKeyword(currentLexeme))
    {
        record.token = "KEYWORD";
    }
    else
    {
        record.token = "IDENTIFIER";
    }
}