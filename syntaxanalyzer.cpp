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

// determines if lexeme is "while"
bool SyntaxAnalyzer::isEndWhile(const Record & lexeme) {
  return (lexeme.token == "KEYWORD") && (lexeme.lexeme == "whileend");
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

bool SyntaxAnalyzer::isOperator(const Record & token, char op) {
    return isOperator(token, string(1, op));
}

bool SyntaxAnalyzer::isOperator(const Record & token, const string & op) {
    return token.token == "OPERATOR" && token.lexeme == op;
}

bool SyntaxAnalyzer::isSeparator(const Record & token, char sep) {
    return token.token == "SEPARATOR" && token.lexeme.length() == 1 && token.lexeme[0] == sep;
}