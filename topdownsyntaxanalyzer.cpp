#include "topdownsyntaxanalyzer.h"
#include "parsetree.h"
#include <iostream>
using namespace std;

ParseTree * TopDownSyntaxAnalyzer::createParseTree() {

    parseTree = new ParseTree();

    currentNode = parseTree->getRoot(); // always add statements to the root node
    bool result = isStatementList();
    if (result)
        cout << "processed program successfully" << endl;
    else {
        Record * token = getCurrentToken();
        cout << "Error: processing program with failure" << endl;
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

    // check for epsilon
    if (inFollowSet(executionFollowSet, nextToken->lexeme)) {
    //if (nextToken->lexeme == "$" || nextToken->lexeme == ";" || nextToken->lexeme == "}") {
        finishNonTerminal(parent);
        return true;
    }
    else if(inFollowSet(statementFollowSet, nextToken->lexeme)) {
    //} else if (record->lexeme == "whileend"|| record->lexeme == "}" || record->lexeme == "enddo" || record->lexeme == "endif" || record->lexeme == "$") {
          //ERIC: the check the follow set to satisfy epsilon
          print("<MoreStatements> -> epsilon");
          //backup();
          finishNonTerminal(parent);
          return true; //just ; is accepted
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
  print("<while statement> -> <WHILE>  <Conditional> <Do> <StatementList> <WhileEnd|EndDo> || <While> <Conditional> { <StatementList> }");
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<while statement> -> <WHILE>  <Conditional> <Do> <StatementList> <WhileEnd|EndDo> || <While> <Conditional> { <StatementList> }");
  if (record == nullptr) {return false;}
  if (isWhile(*record)) {
    currentNode->add(new Node(*record));
    if (isConditionalTopDown()) {
      Record * record = getNextToken();

      if (inFollowSet(conditionalFollowSet, record->lexeme)) {
        currentNode->add(new Node(*record));
        //make sure we implement isStatementList using getNextToken
        if (isStatementList()) {
          Record * record = getNextToken();
          if (inFollowSet(executionFollowSet, record->lexeme)) {
          // if (isEndDo(*record) || isWhileEnd(*record)) {
            currentNode->add(new Node(*record));
            print("<while statement> -> <WHILE>  <Conditional> <Do|{> <StatementList> <WhileEnd|EndDo|}>");
            finishNonTerminal(parent);
            return true;
          }//else {backup();}
          //currentNode->add(new Node(*record));
        }
      }
    /*make sure we do getnextToken in isConditionalTopDown function*/
    // if (isConditionalTopDown()) {
    //   Record * record = getNextToken();
    //   // we have verified while-conditional-...
    //   if (isDo(*record)) { //first variation do-statements-enddo/whileend
    //     currentNode->add(new Node(*record));
    //     //make sure we implement isStatementList using getNextToken
    //     if (isStatementList()) {
    //       Record * record = getNextToken();
    //       if (isEndDo(*record) || isWhileEnd(*record)) {
    //         currentNode->add(new Node(*record));
    //         print("<while statement> -> <WHILE>  <Conditional> <Do> <StatementList> <WhileEnd|EndDo>");
    //         finishNonTerminal(parent);
    //         return true;
    //       }//else {backup();}
    //       //currentNode->add(new Node(*record));
    //     }
    //
    //   } else {
    //     lastError = string("missing keyword 'do' after ").append(record->lexeme);
    //   }//else {backup();}
      /*if (isOpenBracket(*record)) {//second variation { <StatementList> }
        if(isStatementList()) {
          Record * record = getNextToken();
          if(isCloseBracket(*record)) {
            currentNode->add(new Node(*record));
            print("while statement -> <While> <Conditional> { <StatementList> }");
            currentNode->add(new Node(*record));
            finishNonTerminal(parent);
            return true;
          }else {backup();}
          currentNode->add(new Node(*record));
        }
        currentNode->add(new Node(*record));
      }else {backup();}*/
     //currentNode->add(new Node(*record));
    }
  }
//  backup();
  cancelNonTerminal(parent);
  return false;
}
bool TopDownSyntaxAnalyzer::isIfTopDown() {
  print("<if statement> -> <IF> <Conditional> <Do|Then> <StatementList> <EndDo|EndIf> || <IF> <Conditional> { <StatementList> }");
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<if statement> -> <IF> <Conditional> <Do|Then> <StatementList> <EndDo|EndIf> || <IF> <Conditional> { <StatementList> }");
  if (record == nullptr) {return false;}
  if (isIf(*record)) {
    currentNode->add(new Node(*record));
    if (isConditionalTopDown()) {
      Record * record = getNextToken();

      if (inFollowSet(conditionalFollowSet, record->lexeme)) {
        currentNode->add(new Node(*record));
        //make sure we implement isStatementList using getNextToken
        if (isStatementList()) {
          Record * record = getNextToken();
          if (inFollowSet(executionFollowSet, record->lexeme)) {
          // if (isEndDo(*record) || isWhileEnd(*record)) {
            currentNode->add(new Node(*record));
            print("<while statement> -> <IF>  <Conditional> <Do|{> <StatementList> <EndIf|EndDo|}>");
            finishNonTerminal(parent);
            return true;
          }//else {backup();}
          //currentNode->add(new Node(*record));
        }
      }

    }
  }
  //backup();
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
    // Record * token = getNextToken();
    // if (token == nullptr) return false;
    // if(inFollowSet(statementFollowSet, token->lexeme)) {
    //   print("<StatementList>");
    //   finishNonTerminal(parent);
    // }
  }
  // Record * token = getNextToken();
  // if (token == nullptr) return false;
  // if(inFollowSet(statementFollowSet, token->lexeme)) {
  //   print("<StatementList>");
  //   finishNonTerminal(parent);
  // }

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
      // else {
      //   Record * record = getNextToken();
      //   if (isSemiColon(*record)) {
      //     print("MoreStatements -> ; <Statement> <MoreStatements>");
      //     currentNode->add(new Node(*record));
      //     finishNonTerminal(parent);
      //     return true;
      //   }
      //  else {backup();}
      // }
    }
  // if (inFollowSet(statementFollowSet(*record)) {
  //   print ("statementFollowSet");
  //   finishNonTerminal(parent);
  //   return true;
  // }
} else if(inFollowSet(statementFollowSet, record->lexeme)) {
//} else if (record->lexeme == "whileend"|| record->lexeme == "}" || record->lexeme == "enddo" || record->lexeme == "endif" || record->lexeme == "$") {
      //ERIC: the check the follow set to satisfy epsilon
      print("<MoreStatements> -> epsilon");
      backup();
      finishNonTerminal(parent);
      return true; //just ; is accepted
  }
  if (inFollowSet(executionFollowSet, record->lexeme)) {
  //if (nextToken->lexeme == "$" || nextToken->lexeme == ";" || nextToken->lexeme == "}") {
      backup();
      finishNonTerminal(parent);
      return true;
  }
  backup();
  cancelNonTerminal(parent);
  return false;
}
bool TopDownSyntaxAnalyzer::isConditionalTopDown() {
    Node * parent = startNonTerminal("<Conditional> -> <Expression> <RelativeOperator> <Expression>");
    print ("Contidional -> Expression");
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
    print("<Declaration> -> <Type><ID>");
    print("<Type> -> float | int | bool");
    print("<ID> -> identifier");
    Node * parent = startNonTerminal("<Declaration> -> <Type><ID>");
    if (isTypeTopDown()) {
        if (isIdentifier()) {
            symbolTable.add(lexemes[currentLexeme-2], lexemes[currentLexeme-1]);
            /*Record * record = getNextToken();
            if (record == nullptr)
                return false;
            if (record->lexeme == ";") {*/
                //currentNode->add(new Node(record));
                print("<Declaration> -> <Type><ID>");
                finishNonTerminal(parent);
                return true; //success / processed
            //}
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

/**
 * Determines if the next token is an identifier   -- (Sean) changed this to match ^^ isTypeTopDown
 */
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
            // report error here to the error handler
            // allow end of this function to cancel
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
    //cout << "Error: " << currentToken.filename->c_str() << ":" << currentToken.line << ":" << currentToken.linePosition 
    //     << currentProduction << " was not met" << endl;
         
    delete currentNode;
    currentNode = parent;
}

bool TopDownSyntaxAnalyzer::isQ(){
    Record * record = getNextToken();
    if (record == nullptr)
        return false;
    Node * parent = startNonTerminal("<ExpressionPrime> -> +<Term><ExpressionPrime> | -<Term><ExpressionPrime> | epsilon"/*, *record*/);

    if (record->lexeme == "+") {
        currentNode->add(new Node(*record));
        if (isT()) {
            if (isQ()) {
                //cout << " Q -> +TQ" << endl;
                print("<ExpressionPrime> -> +<Term><ExpressionPrime>");
                finishNonTerminal(parent);
                return true;
            }
        }
    } else if (record->lexeme == "-") {
        currentNode->add(new Node(*record));
        if (isT()) {
            if (isQ()) {
                //cout << " Q -> +TQ" << endl;
                print("<ExpressionPrime> -> -<Term><ExpressionPrime>");
                finishNonTerminal(parent);
                return true;
            }
        }
      //}
    //} else if (record->lexeme == "whileend" || record->lexeme == "do" || record->lexeme == "<" || record->lexeme == ")" || record->lexeme == ";") {
    } else if(inFollowSet(expressionPrimeFollowSet, record->lexeme)) {
        // if (isT()) {
        //   if (isQ()) {
        //     print("<ExpressionPrime> -> <Term><ExpressionPrime>");
        //     finishNonTerminal(parent);
        //     return true;
        //   }
        // }
        //                 //cout << " Q -> epsilon" << endl;
        print("<ExpressionPrime> -> epsilon");
        backup();
        finishNonTerminal(parent);
        return true;
    }
    cancelNonTerminal(parent);
    return false;
}

bool TopDownSyntaxAnalyzer::isT() {
    //print(" <Term> -> <Factor><TermPrime>");
    Node * parent = startNonTerminal("<Term> -> <Factor><TermPrime>");
    if (isF()) {
        if (isR()) {
            //cout << " T -> FR" << endl;
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
                //cout << *record << endl;
                //cout << " R -> *FR" << endl;
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
        //record->lexeme == "whileend" || record->lexeme == "do" || record->lexeme == "<" || record->lexeme == "+" || record->lexeme == "-" || record->lexeme == ")" || record->lexeme == ";") {
        //cout << *record << endl;
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
    //print(" <Factor> -> <Identifier>");
    if (isId(*record) && isIdentifier(true)) {
        //cout << *record << endl;
        //cout << " F -> id" << endl;
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
                    //cout << " F -> (E)" << endl;
                    print("<Factor> -> (<Expression>)");
                    finishNonTerminal(parent);
                    return true;
                }
            }
    }
    //} else if (record->lexeme == "whileend" || record->lexeme == "do" || record->lexeme == "<" || record->lexeme == ")" || record->lexeme == ";") {
    // } else if(inFollowSet(expressionPrimeFollowSet, record->lexeme)) {
    //     // if (isT()) {
    //     //   if (isQ()) {
    //     //     print("<ExpressionPrime> -> <Term><ExpressionPrime>");
    //     //     finishNonTerminal(parent);
    //     //     return true;
    //     //   }
    //     // }
    //     //                 //cout << " Q -> epsilon" << endl;
    //     print("<ExpressionPrime> -> epsilon");
    //     finishNonTerminal(parent);
    //     return true;
    // }

    cancelNonTerminal(parent);
    return false;
}

bool TopDownSyntaxAnalyzer::isE() {
    //print(" <Expression> -> <Term><ExpressionPrime>");
    //cout << *currentLexeme << endl;
    Node * parent = startNonTerminal("<Expression> -> <Term><ExpressionPrime>");
    if (isT()) {
        if (isQ()) {
            /*Record * record = getNextToken();
            if (record == nullptr)
                return false;

            // the ; optionally ends a statement
            if (record->lexeme != ";")
                backup();
*/
            //cout << " E -> TQ" << endl;
            print("<Expression> -> <Term><ExpressionPrime>");
            finishNonTerminal(parent);
            return true;
        }
    }
    cancelNonTerminal(parent);
    return false;
}

bool TopDownSyntaxAnalyzer::isAssignment() {
    //cout << *currentLexeme << endl;
    //print(" <Assign> -> <ID> = <Expression>");
    Node * parent = startNonTerminal("<Assign> -> <ID> = <Expression>");
    if (isIdentifier(true)) {
        Record * record = getNextToken();
        if (record == nullptr)
            return false;
        //cout << * record << endl;
        if (record->lexeme == "=") {
            currentNode->add(new Node(*record));
            if(isE()) {
                //Record * record = getNextToken();
                //if (record == nullptr)
                //    return false;
                //cout << * record << endl;
                // the ; optionally ends a statement
                //if (record->lexeme != ";")
                //    backup();

                //cout << " <Assign> -> <ID> = <Expression>;" << endl;
                //print("<Assign> -> <ID> = <Expression>");
                //currentNode->add(new Node(*record));
                finishNonTerminal(parent);
                return true;
            }
        }
    }
    cancelNonTerminal(parent);
    return false;
}
