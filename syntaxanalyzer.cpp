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
bool SyntaxAnalyzer::isThen(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.token == "then";
}
bool SyntaxAnalyzer::isWhileEnd(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.token == "whileend";
}
bool SyntaxAnalyzer::isDo(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.token == "do";
}
bool SyntaxAnalyzer::isEndIf(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.token == "endif";
}
bool SyntaxAnalyzer::isEndDo(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.token == "enddo";
}
bool SyntaxAnalyzer::isOpenBracket(const Record & lexeme) {
    return lexeme.token == "SEPERATOR" && lexeme.token == "{";
}
bool SyntaxAnalyzer::isCloseBracket(const Record & lexeme) {
    return lexeme.token == "SEPERATOR" && lexeme.token == "}";
}
bool SyntaxAnalyzer::isElse(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.token == "else";
}
bool SyntaxAnalyzer::isEndElse(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.token == "endelse";
}
