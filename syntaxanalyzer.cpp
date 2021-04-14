#include "syntaxanalyzer.h"

/**
 * returns true  if a lexeme is a type
 */
bool SyntaxAnalyzer::isType(const Record & lexeme) {
    return lexeme.token == "KEYWORD" &&
        (lexeme.lexeme == "int" || lexeme.lexeme == "float" || lexeme.lexeme == "bool");
}

// returns true if lexeme is "if"
bool SyntaxAnalyzer::isIf(const Record & lexeme) {
  return (lexeme.token == "KEYWORD") && (lexeme.lexeme == "if");
}
// returns true if lexeme is "while"
bool SyntaxAnalyzer::isWhile(const Record & lexeme) {
  return (lexeme.token == "KEYWORD") && (lexeme.lexeme == "while");
}
/**
 * returns true  if a lexeme is an identifier
 */
bool SyntaxAnalyzer::isId(const Record & lexeme) {
    return lexeme.token == "IDENTIFIER";
}

//returns true if lexeme is a number ("integer" or "float")
bool SyntaxAnalyzer::isNumber(const Record & lexeme) {
    return lexeme.token == "INTEGER" || lexeme.token == "FLOAT";
}

//returns true if lexeme is "then"
bool SyntaxAnalyzer::isThen(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "then";
}

//returns true if lexeme is "whileend"
bool SyntaxAnalyzer::isWhileEnd(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "whileend";
}

//returns true if lexeme is "do"
bool SyntaxAnalyzer::isDo(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "do";
}

//returns true if lexeme is "endif"
bool SyntaxAnalyzer::isEndIf(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "endif";
}

//returns true if lexeme is "enddo"
bool SyntaxAnalyzer::isEndDo(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "enddo";
}

//returns true if lexeme is "{" (open bracket)
bool SyntaxAnalyzer::isOpenBracket(const Record & lexeme) {
    return lexeme.token == "SEPERATOR" && lexeme.lexeme == "{";
}

//returns true if lexeme is "}" (close bracket)
bool SyntaxAnalyzer::isCloseBracket(const Record & lexeme) {
    return lexeme.token == "SEPERATOR" && lexeme.lexeme == "}";
}

//returns true if the lexeme is "else"
bool SyntaxAnalyzer::isElse(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "else";
}

//returns true if the lexeme is "endelse"
bool SyntaxAnalyzer::isEndElse(const Record & lexeme) {
    return lexeme.token == "KEYWORD" && lexeme.lexeme == "endelse";
}

//returns true if the lexeme is a semicolon (";")
bool SyntaxAnalyzer::isSemiColon(const Record & lexeme) {
    return lexeme.token == "SEPARATOR" && lexeme.lexeme == ";";
}

//returns true if the lexeme is an operator
bool SyntaxAnalyzer::isOperator(const Record & lexeme) {
    return lexeme.token == "OPERATOR";
}
