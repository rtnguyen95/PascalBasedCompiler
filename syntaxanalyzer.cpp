#include "syntaxanalyzer.h"

/**
 * Determines if a lexeme is a type
 */
bool SyntaxAnalyzer::isType(const Record & lexeme) {
    return lexeme.token == "KEYWORD" &&
        (lexeme.lexeme == "int" || lexeme.lexeme == "float" || lexeme.lexeme == "bool");
}

// determines if lexeme is "if"
bool SyntaxAnalyzer::isIf(const Record & lexeme) {
  return (lexeme.token == "KEYWORD") && (lexeme.lexeme == "if");
}
// determines if lexeme is "while"
bool SyntaxAnalyzer::isWhile(const Record & lexeme) {
  return (lexeme.token == "KEYWORD") && (lexeme.lexeme == "while");
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
