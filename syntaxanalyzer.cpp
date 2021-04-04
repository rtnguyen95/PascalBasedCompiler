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


bool SyntaxAnalyzer::isNumber(const Record & lexeme) {
    return lexeme.token == "INTEGER" || lexeme.token == "FLOAT";    
}