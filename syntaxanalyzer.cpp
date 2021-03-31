#include "syntaxanalyzer.h"

/**
 * Determines if a lexeme is a type
 */
bool SyntaxAnalyzer::isType(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && 
        (lexeme.lexeme == "int" || lexeme.lexeme == "float" || lexeme.lexeme == "bool");
}

/**
 * Determines if a lexeme is an identifier
 */
bool SyntaxAnalyzer::isId(const Record & lexeme) {
    return lexeme.token == "IDENTIFIER";
}

/**
 * Determines if the next token is an identifier
 */
bool SyntaxAnalyzer::isIdentifier() {
    Record * record = getNextToken();
    if (record == nullptr) 
        return false;
    return isId(*record);
}


bool SyntaxAnalyzer::isNumber(const Record & lexeme) {
    return lexeme.token == "INTEGER" || lexeme.token == "FLOAT";    
}