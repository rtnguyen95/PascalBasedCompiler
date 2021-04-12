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
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "then";
}
bool SyntaxAnalyzer::isWhileEnd(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "whileend";
}
bool SyntaxAnalyzer::isDo(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "do";
}
bool SyntaxAnalyzer::isEndIf(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "endif";
}
bool SyntaxAnalyzer::isEndDo(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "enddo";
}
bool SyntaxAnalyzer::isOpenBracket(const Record & lexeme) {
    return lexeme.token == "SEPERATOR" && lexeme.lexeme == "{";
}
bool SyntaxAnalyzer::isCloseBracket(const Record & lexeme) {
    return lexeme.token == "SEPERATOR" && lexeme.lexeme == "}";
}
bool SyntaxAnalyzer::isElse(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "else";
}
bool SyntaxAnalyzer::isEndElse(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "endelse";
}
bool SyntaxAnalyzer::isSemiColon(const Record & lexeme) {
    return lexeme.token == "SEPERATOR" && lexeme.lexeme == ";";
}
bool SyntaxAnalyzer::isOperator(const Record & lexeme) {
    return lexeme.token == "OPERATOR";
}
