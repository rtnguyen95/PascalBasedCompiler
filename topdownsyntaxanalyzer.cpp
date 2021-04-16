#include "topdownsyntaxanalyzer.h"
#include "parsetree.h"
#include <iostream>
using namespace std;

//Function to create parse tree
ParseTree * TopDownSyntaxAnalyzer::createParseTree()
{
    parseTree = new ParseTree(); //initialize the parse tree
    currentNode = parseTree->getRoot(); // always add statements to the root node
    bool result = isStatementList();
    if (result)
        cout << "Program Processing Successful" << endl;
    else {
        Record * token = getCurrentToken();
        cout << "Error: Program Processing Failure" << endl;
        string errorMessage = "";
        if (token != nullptr) {
            cout << token->filename->c_str() << ":" << token->line << ":" << token->linePosition << " - ";
            cout << "this rule " << currentProduction << " could not be met with " << token->lexeme << endl;
            errorMessage.append("this rule ").append(currentProduction).append(" could not be met with '").append(token->lexeme)
              .append("'\n").append(lastError);
        }
        errorHandler.addError({token->filename->c_str(), token->line, token->linePosition, errorMessage, syntax_error});
    }
    parseTree->printRules(cout);
    return parseTree;
}
bool TopDownSyntaxAnalyzer::isStatement() {
    Node * parent = startNonTerminal("<Statement> -> <Assign> | <Declaration> | <While> | <If>");
    Record * nextToken = getNextToken();
    if (nextToken == nullptr) return false;
    backup();
    if(isType(*nextToken) && isDeclaration()) {
        finishNonTerminal(parent);
        return true;
    }
    if(isId(*nextToken) && isAssignment()) {
        finishNonTerminal(parent);
        return true;
    }
    if(isWhile(*nextToken) && isWhileTopDown()) {
        finishNonTerminal(parent);
        return true;
    }
    if(isIf(*nextToken) && isIfTopDown()) {
        finishNonTerminal(parent);
        return true;
    }
    if(isElse(*nextToken) && isElseTopDown()) {
      finishNonTerminal(parent);
      return true;
    }
    // check for epsilon
    if (inFollowSet(executionFollowSet, nextToken->lexeme)) {
        finishNonTerminal(parent);
        return true;
    }
    else if(inFollowSet(statementFollowSet, nextToken->lexeme)) {
          print("<MoreStatements> -> epsilon");
          finishNonTerminal(parent);
          return true;
      }
    cancelNonTerminal(parent);
    return false;
}
bool TopDownSyntaxAnalyzer::isBoolValueTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<BooleanValue> -> True | False");
  if (record == nullptr)
    return false;
  if (isBoolValue(*record)) {
    currentNode->add(new Node(*record));
    finishNonTerminal(parent);
    return true;
  }
  cancelNonTerminal(parent);
  return false;
}
bool TopDownSyntaxAnalyzer::isNumberTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<NUM> -> number");
  if (record == nullptr)
    return false;
  if (isNumber(*record)) {
    currentNode->add(new Node(*record));
    finishNonTerminal(parent);
    return true;
  }
  cancelNonTerminal(parent);
  return false;
}
bool TopDownSyntaxAnalyzer::isWhileTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<While> -> while <Conditional> do|then|{ <StatementList> whileend|whiledo|}");
  if (record == nullptr) {return false;}
  if (isWhile(*record)) {
    currentNode->add(new Node(*record));
    if (isConditionalTopDown()) {
      Record * record = getNextToken();
      if (inFollowSet(conditionalFollowSet, record->lexeme)) {
        currentNode->add(new Node(*record));
        if (isStatementList()) {
          Record * record = getNextToken();
          if (inFollowSet(executionFollowSet, record->lexeme)) {
            currentNode->add(new Node(*record));
            print("<While> -> while <Conditional> do|then|{ <StatementList> whileend|whiledo|}");
            finishNonTerminal(parent);
            return true;
          }
        }
      }
    }
  }
  cancelNonTerminal(parent);
  return false;
}
bool TopDownSyntaxAnalyzer::isIfTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<If> -> if <Conditional> do|then|{ <StatementList> enddo|endif|}");
  if (record == nullptr) {return false;}
  if (isIf(*record)) {
    currentNode->add(new Node(*record));
    if (isConditionalTopDown()) {
      Record * record = getNextToken();
      if (inFollowSet(conditionalFollowSet, record->lexeme)) {
        currentNode->add(new Node(*record));
        if (isStatementList()) {
          Record * record = getNextToken();
          if (inFollowSet(executionFollowSet, record->lexeme)) {
            currentNode->add(new Node(*record));
            print("<if statement> -> <IF>  <Conditional> <Do|Then|{> <StatementList> <EndIf|EndDo|}>");
            finishNonTerminal(parent);
            return true;
          }
        }
      }
    }
  }
  cancelNonTerminal(parent);
  return false;
}
bool TopDownSyntaxAnalyzer::isStatementList() {
  Node * parent = startNonTerminal("<StatementList> -> <Statement> <MoreStatements>");
  if(isStatement()) {
    if(isMoreStatements()){
      print("<StatementList> -> <Statement> <MoreStatements>");
      finishNonTerminal(parent);
      return true;
    }
  }
  cancelNonTerminal(parent);
  return false;
}
bool TopDownSyntaxAnalyzer::isMoreStatements() {
  Node * parent = startNonTerminal("<MoreStatements> -> ; <Statement> <MoreStatements> | epsilon");
  Record * record = getNextToken();
  if(isSemiColon(*record)) {
    currentNode->add(new Node(*record));
    if (isStatement()) {
      if(isMoreStatements()) {
        print("<MoreStatements> -> ; <Statement> <MoreStatements>");
        finishNonTerminal(parent);
        return true;
      }
    }
  } else if(inFollowSet(statementFollowSet, record->lexeme)) {
      print("<MoreStatements> -> epsilon");
      backup();
      finishNonTerminal(parent);
      return true;
  }
  if (inFollowSet(executionFollowSet, record->lexeme)) {
    backup();
    finishNonTerminal(parent);
    return true;
  }
  backup();
  cancelNonTerminal(parent);
  return false;
}
bool TopDownSyntaxAnalyzer::isElseTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<Else> -> else if|while <Conditional> do|then|{ <StatementList> enddo|endelse|}");
  if (record == nullptr) {return false;}
  if (isElse(*record)) {
    currentNode->add(new Node(*record));
    Record * record = getNextToken();
    if (isIf(*record) || isWhile(*record)) {
        currentNode->add(new Node(*record));
        if (isConditionalTopDown()) {
          Record * record = getNextToken();
          if (inFollowSet(conditionalFollowSet, record->lexeme)) {
            currentNode->add(new Node(*record));
            if (isStatementList()) {
              Record * record = getNextToken();
              if (inFollowSet(executionFollowSet, record->lexeme)) {
                currentNode->add(new Node(*record));
                print("<Else> -> else if|while <Conditional> do|then|{ <StatementList> enddo|endelse|}");
                finishNonTerminal(parent);
                return true;
              }
            }
          }
        }
    }
  }
  cancelNonTerminal(parent);
  return false;
}

bool TopDownSyntaxAnalyzer::isConditionalTopDown() {
    Node * parent = startNonTerminal("<Conditional> -> (<Expression> <Relational Operator> <Expression>) | <Expression> <Relational Operator> <Expression>");
    Record * record = getNextToken();
    if (record->lexeme == "(") {
            currentNode->add(new Node(*record));
            if (isE()) {
              Record * token = getNextToken();
              if (token == nullptr) return false;
               if (isRelativeOperator(*token)) {
                  currentNode->add(new Node(token));
                  if (isE()) {
                      Record * record = getNextToken();
                      if (record->lexeme == ")") {
                        currentNode->add(new Node(token));
                        print("<Conditional> -> (<Expression> <Relational Operator> <Expression>) || <Expression> <Relational Operator> <Expression>");
                        finishNonTerminal(parent);
                        return true;
                      }
                  }
              }
            }
    }else {backup();}
    if (isE()) {
        Record * token = getNextToken();
        if (token == nullptr) return false;
         if (isRelativeOperator(*token)) {
            currentNode->add(new Node(token));
            if (isE()) {
                finishNonTerminal(parent);
                return true;
            }
        } else if(inFollowSet(expressionFollowSet, token->lexeme)) {
            backup();
            finishNonTerminal(parent);
            return true;
        }
    }
    cancelNonTerminal(parent);
    return false;
}
bool TopDownSyntaxAnalyzer::isDeclaration() {
    Node * parent = startNonTerminal("<Declaration> -> <Type><ID>");
    if (isTypeTopDown()) {
        if (isIdentifier()) {
            symbolTable.add(lexemes[currentLexeme-2], lexemes[currentLexeme-1]);
                print("<Declaration> -> <Type><ID>");
                finishNonTerminal(parent);
                return true;
        }
    }
    cancelNonTerminal(parent);
    return false;
}
bool TopDownSyntaxAnalyzer::isTypeTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<Type> -> float | int | bool");
  if (record == nullptr)
    return false;
  if (isType(*record)) {
    currentNode->add(new Node(*record));
    finishNonTerminal(parent);
    return true;
  }
  cancelNonTerminal(parent);
  return false;
}
bool TopDownSyntaxAnalyzer::isIdentifier(bool check) {
    Record * record = getNextToken();
    Node * parent = startNonTerminal("<ID> -> identifier");
    if (record == nullptr) {return false;}
    if(isId(*record)) {
        if (!check || symbolTable.exists(record->lexeme)) {
            currentNode->add(new Node(*record));
            finishNonTerminal(parent);
            return true;
        } else {
            errorHandler.addError(Error(*record, record->lexeme + " was not previously defined", syntax_error));
        }
    }
    cancelNonTerminal(parent);
    return false;
}
Node * TopDownSyntaxAnalyzer::startNonTerminal(const string & name) {
    Node * parent = currentNode;
    currentProduction = name;
    print(name);
    currentNode = new Node(name);
    return parent;
}

void TopDownSyntaxAnalyzer::finishNonTerminal(Node * parent) {
    parent->add(currentNode);
    currentNode = parent;
}

void TopDownSyntaxAnalyzer::cancelNonTerminal(Node * parent) {
    delete currentNode;
    currentNode = parent;
}

bool TopDownSyntaxAnalyzer::isQ(){
    Record * record = getNextToken();
    if (record == nullptr)
        return false;
    Node * parent = startNonTerminal("<ExpressionPrime> -> +<Term><ExpressionPrime> | -<Term><ExpressionPrime> | epsilon");
    if (record->lexeme == "+") {
        currentNode->add(new Node(*record));
        if (isT()) {
            if (isQ()) {
                print("<ExpressionPrime> -> +<Term><ExpressionPrime>");
                finishNonTerminal(parent);
                return true;
            }
        }
    } else if (record->lexeme == "-") {
        currentNode->add(new Node(*record));
        if (isT()) {
            if (isQ()) {
                print("<ExpressionPrime> -> -<Term><ExpressionPrime>");
                finishNonTerminal(parent);
                return true;
            }
        }
    } else if(inFollowSet(expressionPrimeFollowSet, record->lexeme)) {
        print("<ExpressionPrime> -> epsilon");
        backup();
        finishNonTerminal(parent);
        return true;
    }
    cancelNonTerminal(parent);
    return false;
}
bool TopDownSyntaxAnalyzer::isT() {
    Node * parent = startNonTerminal("<Term> -> <Factor><TermPrime>");
    if (isF()) {
        if (isR()) {
            print("<Term> -> <Factor><TermPrime>");
            finishNonTerminal(parent);
            return true;
        }
    }
    cancelNonTerminal(parent);
    return false;
}
bool TopDownSyntaxAnalyzer::isR() {
    Record * record = getNextToken();
    if (record == nullptr)
        return false;
    Node * parent = startNonTerminal("<TermPrime> -> *<Factor><TermPrime> | /<Factor><TermPrime> | epsilon");
    if (record->lexeme == "*") {
        currentNode->add(new Node(*record));
        if (isF()) {
            if (isR()) {
                print("<TermPrime> -> *<Factor><TermPrime>");
                finishNonTerminal(parent);
                return true;
            }
        }
    } else if(record->lexeme == "/") {
        currentNode->add(new Node(*record));
        if (isF()) {
            if (isR()) {
                print("<TermPrime> -> /<Factor><TermPrime>");
                finishNonTerminal(parent);
                return true;
            }
        }
    } else if (inFollowSet(termPrimeFollowSet, record->lexeme)) {
        print("<TermPrime> -> epsilon");
        backup();
        finishNonTerminal(parent);
        return true;
    }
    cancelNonTerminal(parent);
    return false;
}
bool TopDownSyntaxAnalyzer::isF() {
    Record * record = getNextToken();
    if (record == nullptr)
        return false;
    backup();
    Node * parent = startNonTerminal("<Factor> -> (<Expression>) | <ID> | <NUM>");
    if (isId(*record) && isIdentifier(true)) {
        print("<Factor> -> <Identifier>");
        finishNonTerminal(parent);
        return true;
    }
    if (isNumber(*record) && isNumberTopDown()) {
      print("<Factor> -> <NUM>");
      finishNonTerminal(parent);
      return true;
    }
    if (record->lexeme == "(") {
            getNextToken();
            currentNode->add(new Node(*record));
            if (isE()) {
                Record * record = getNextToken();
                if (record == nullptr)
                    return false;
                if (record->lexeme == ")") {
                    currentNode->add(new Node(*record));
                    print("<Factor> -> (<Expression>)");
                    finishNonTerminal(parent);
                    return true;
                }
            }
    }
    cancelNonTerminal(parent);
    return false;
}

bool TopDownSyntaxAnalyzer::isE() {
    Node * parent = startNonTerminal("<Expression> -> <Term><ExpressionPrime> || <BooleanValue>");
    Record * token = lookAhead();
    if (isBoolValue(*token) && isBoolValueTopDown()) {
      print("<Expression> -> <Boolean Value>");
      finishNonTerminal(parent);
      return true;
    } else if (isT()) {
        if (isQ()) {
            print("<Expression> -> <Term><ExpressionPrime>");
            finishNonTerminal(parent);
            return true;
        }
    }
    
    cancelNonTerminal(parent);
    return false;
}
bool TopDownSyntaxAnalyzer::isAssignment() {
    Node * parent = startNonTerminal("<Assign> -> <ID> = <Expression>");
    if (isIdentifier(true)) {
        Record * record = getNextToken();
        if (record == nullptr)
            return false;
        if (record->lexeme == "=") {
            currentNode->add(new Node(*record));
            if(isE()) {
                finishNonTerminal(parent);
                return true;
            }
        }
    }
    cancelNonTerminal(parent);
    return false;
}
