#include "topdownsyntaxanalyzer.h"
#include "parsetree.h"
#include <iostream>
using namespace std;

//Function to create parse tree
ParseTree * TopDownSyntaxAnalyzer::createParseTree()
{
    parseTree = new ParseTree(); //initialize the parse tree
    currentNode = parseTree->getRoot(); // always add statements to the root node
    
    //create a boolean variable called result and assign it to the outcome of isStatementList. result will return true if processing is successful and false if processing fails
    bool result = isStatementList();
    
    //if processing is successful output a confirmation to the console
    if (result)
        cout << "Program Processing Successful" << endl;
   
    //else if processing fails
    else {
        //get the token that has failed processing
        Record * token = getCurrentToken();
        
        //output an error message to the console
        cout << "Error: Program Processing Failure" << endl;
        
        //create a string variable errorMessage that will hold information about the error
        string errorMessage = "";
        
        //check to see if token is not null
        if (token != nullptr)
        {
            //output the name of the file, line number, and line position number where the error was found to the console
            cout << token->filename->c_str() << ":" << token->line << ":" << token->linePosition << " - ";
            
            //output the rule and the lexeme that mismatched to the console
            cout << "this rule " << currentProduction << " could not be met with " << token->lexeme << endl;
            
            //add information about the rule and the lexeme that mismatched to errorMessage
            errorMessage.append("this rule ").append(currentProduction).append(" could not be met with '").append(token->lexeme)
              .append("'\n").append(lastError);
        }
        
        //pass the file name, line number, line position number, error message string, and syntax_error information to errorHandler.addError so it can be recorded
        errorHandler.addError({token->filename->c_str(), token->line, token->linePosition, errorMessage, syntax_error});
    }
    
    parseTree->printRules(cout); //print the rules of the parseTree
    return parseTree; //return parseTree to the caller
}

/*
 <Statement> -> <Assign> | <Declaration> | <While> | <If> | <Else> function
 Checks to see if a token satisfies the <Statement> rule.
 This function uses a predictive approach using FIRST sets.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isStatement() {
    //create a node for the parse tree for this rule
    Node * parent = startNonTerminal("<Statement> -> <Assign> | <Declaration> | <While> | <If> | <Else>");
    
    //get the Record object for the next token and point to it with a variable called nextToken
    Record * nextToken = getNextToken();
    
    //if nextToken is null return the function as false
    if (nextToken == nullptr) return false;
    
    //undo the "peek" so it does not affect the variables in future processing
    backup();
    
    //check to see if the non-terminal satisfies <Statement> -> <Declarative> using FIRST(Declarative) = Type
    if(isType(*nextToken) && isDeclaration()) {
        //if true add the node to the parse tree and return the function as true
        finishNonTerminal(parent);
        return true;
    }
    
    //check to see if the non-terminal satisfies <Statement> -> <Assign> using FIRST(Assign) = ID
    if(isId(*nextToken) && isAssignment()) {
        //if true add the node to the parse tree and return the function as true
        finishNonTerminal(parent);
        return true;
    }
    
    //check to see if the non-terminal satisfies <Statement> -> <While> using FIRST(While) = while
    if(isWhile(*nextToken) && isWhileTopDown()) {
        //if true add the node to the parse tree and return the function as true
        finishNonTerminal(parent);
        return true;
    }
    
    //check to see if the non-terminal satisfies <Statement> -> <If> using FIRST(If) = if
    if(isIf(*nextToken) && isIfTopDown()) {
        //if true add the node to the parse tree and return the function as true
        finishNonTerminal(parent);
        return true;
    }
    
    //check to see if the non-terminal satisfies <Statement> -> <Else> using FIRST(Else) = else
    if(isElse(*nextToken) && isElseTopDown()) {
        //if true add the node to the parse tree and return the function as true
      finishNonTerminal(parent);
      return true;
    }
    
    // check for epsilon
    if (inFollowSet(executionFollowSet, nextToken->lexeme)) {
        //if true add the node to the parse tree and return the function as true
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

/*<Boolean Value> -> <True> || <False> function
Checks to see if a token satisfies the <Boolean Value> rule.
Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isBoolValueTopDown() {
  //get the Record object that holds the information for the next token to be processed and point to the record with a pointer called record
    Record * record = getNextToken();
    
    //creates a node for the parse tree for this rule
    Node * parent = startNonTerminal("<Boolean Value> -> <True> || <False>");
    
    //if record is nullptr then the function returns false
    if (record == nullptr)
        return false;
    
    //check to see if the lexeme is true or false
    if (isBoolValue(*record))
    {
        //if true the rule is satisfied. Add the Record to the token to the parse tree and return true to the caller
        currentNode->add(new Node(*record));
        finishNonTerminal(parent);
        return true;
    }
    
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
    cancelNonTerminal(parent);
    return false;
}

/*
 <NUM> -> number function
 Checks to see if a token satisfies the <NUM> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
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

/*
 <while statement> -> <WHILE>  <Conditional> <Do|Then> <StatementList> <WhileEnd|EndDo> || <While> <Conditional> { <StatementList> } function
 Checks to see if a token satisfies the <while> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isWhileTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<While> -> while <Conditional> do|then|{ <StatementList> whileend|whiledo|}");
  if (record == nullptr) {return false;}

  //  check for non-terminals: <While>, <Conditional>, FOLLOW(Conditional), <StatementList>, FOLLOW(StatementList), consecutively
    //if all return true then the node is added to the tree and the function returns true
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
    
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
  cancelNonTerminal(parent);
  return false;
}


/*
 <if statement> -> <IF> <Conditional> <Do|Then> <StatementList> <EndDo|EndIf> || <IF> <Conditional> { <StatementList> }
 Checks to see if a token satisfies the <if statement> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isIfTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<If> -> if <Conditional> do|then|{ <StatementList> enddo|endif|}");
  if (record == nullptr) {return false;}
    
    //  check for non-terminals: <IF>, <Conditional>, FOLLOW(Conditional), <StatementList>, FOLLOW(StatementList), consecutively
    //if all return true then the node is added to the tree and the function returns true
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
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
  cancelNonTerminal(parent);
  return false;
}


/*
 <StatementList> -> <Statement> <MoreStatements>
 Checks to see if a token satisfies the <StatementList> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isStatementList() {
  Node * parent = startNonTerminal("<StatementList> -> <Statement> <MoreStatements>");
    
    //check for non-terminals: <Statement>, <MoreStatements>, consecutively
    //if all return true then the node is added to the tree and the function returns true
  if(isStatement()) {
    if(isMoreStatements()){
      print("<StatementList> -> <Statement> <MoreStatements>");
      finishNonTerminal(parent);
      return true;
    }
  }
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
  cancelNonTerminal(parent);
  return false;
}


/*
 <MoreStatements> -> ; <Statement> <MoreStatements> | epsilon
 Checks to see if a token satisfies the <MoreStatements> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isMoreStatements() {
  Node * parent = startNonTerminal("<MoreStatements> -> ; <Statement> <MoreStatements> | epsilon");
  Record * record = getNextToken();
    
    //check for non-terminals: ; <Statement>, <MoreStatements>, consecutively
    //if all return true then the node is added to the tree and the function returns true
  if(isSemiColon(*record)) {
    currentNode->add(new Node(*record));
    if (isStatement()) {
      if(isMoreStatements()) {
        print("<MoreStatements> -> ; <Statement> <MoreStatements>");
        finishNonTerminal(parent);
        return true;
      }
    }
      
      //else if check for epsilon
      //if all true then the node is added to the tree and the function returns true
  } else if(inFollowSet(statementFollowSet, record->lexeme)) {
      print("<MoreStatements> -> epsilon");
      backup();
      finishNonTerminal(parent);
      return true;
  }
    
    //check for execution Follow set and if true then the node is added to the tree and the function returns true
  if (inFollowSet(executionFollowSet, record->lexeme)) {
    backup();
    finishNonTerminal(parent);
    return true;
  }
    
//if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
  backup();
  cancelNonTerminal(parent);
  return false;
}

/*
 <else statement> -> <else> <If|While> <Conditional> <Do|Then> <StatementList> <EndDo|EndElse> || <Else> <Conditional> { <StatementList> }
 Checks to see if a token satisfies the <else statement> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isElseTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<Else> -> else if|while <Conditional> do|then|{ <StatementList> enddo|endelse|}");
  if (record == nullptr) {return false;}
    
    //checks for <else>, <if> or <while>, <conditional>, Follow(conditional), <StatementList>, Follow(StatementList), consecutively
    //if all return true then the node is added to the tree and the function returns true
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
    
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
  cancelNonTerminal(parent);
  return false;
}


/*
 <Conditional> -> (<Expression> <Relational Operator> <Expression>) || <Expression> <Relational Operator> <Expression> function
 Checks to see if a token satisfies the <Conditional> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isConditionalTopDown() {
    Node * parent = startNonTerminal("<Conditional> -> (<Expression> <Relational Operator> <Expression>) | <Expression> <Relational Operator> <Expression>");
    Record * record = getNextToken();
    
    //checks for (, <Expression>, <Relational Operator>, <Expression>, ), consecutively
    //if all return true the node is added to the tree and the function returns true
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
    //else backup is called and the function checks again for:
    //<Expression>, <Relational Operator>, <Expression>, consecutively
    //if all return true the node is added to the tree and the function returns true
    if (isE()) {
        Record * token = getNextToken();
        if (token == nullptr) return false;
         if (isRelativeOperator(*token)) {
            currentNode->add(new Node(token));
            if (isE()) {
                finishNonTerminal(parent);
                return true;
            }
        }
        //else if the function checks Follow(Expression) and if truethe node is added to the tree and the function returns true
         else if(inFollowSet(expressionFollowSet, token->lexeme)) {
            backup();
            finishNonTerminal(parent);
            return true;
        }
    }
    cancelNonTerminal(parent);
    return false;
}

/* <Declaration> -> <Type><ID> function
 Checks to see if a token satisfies the <Declaration> rule.
Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isDeclaration() {
    
    //print the rules associated with Declaration
    print("<Declaration> -> <Type><ID>");
    print("<Type> -> float | int | bool");
    print("<ID> -> identifier");
    
    //creates a node for the parse tree for this rule
    Node * parent = startNonTerminal("<Declaration> -> <Type><ID>");
    
    //check for two non-terminals: <Type> and <ID>, consecutively
    if (isTypeTopDown()) {
        if (isIdentifier()) {
            //if both Type and Id are found add both lexemes to the symbol table, print the rule, finish adding the node for the rule to the parse tree, and return the function as true to the caller
            symbolTable.add(lexemes[currentLexeme-2], lexemes[currentLexeme-1]);
                print("<Declaration> -> <Type><ID>");
                finishNonTerminal(parent);
                gen_instr("POPM", to_string(symbolTable.getAddress(lexemes[currentLexeme-1].lexeme)));
                return true;
        }
    }
    
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
    cancelNonTerminal(parent);
    return false;
}

/* <Type> -> float | int | bool function
 Checks to see if a token satisfies the <Type> rule.
Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isTypeTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("<Type> -> float | int | bool");
  if (record == nullptr)
    return false;
    //checks for float, int, or bool and if true adds the node to the tree and the function returns true
  if (isType(*record)) {
    currentNode->add(new Node(*record));
    finishNonTerminal(parent);
    return true;
  }
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
  cancelNonTerminal(parent);
  return false;
}

/* <ID> -> identifier function
 Checks to see if a token satisfies the <ID> rule.
Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isIdentifier(bool check) {
    Record * record = getNextToken();
    Node * parent = startNonTerminal("<ID> -> identifier");
    if (record == nullptr) {return false;}
    //checks for an identifier
    if(isId(*record)) {
        //checks the symbol table to see if the identifier was defined the program. If true, the node is added to the tree and the function returns true
        if (!check || symbolTable.exists(record->lexeme)) {
            currentNode->add(new Node(*record));
            finishNonTerminal(parent);
            return true;
        }
        //else an error is recorded because the identifier was not previously defined before being used in the program
        else {
            errorHandler.addError(Error(*record, record->lexeme + " was not previously defined", syntax_error));
        }
    }
    
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
    cancelNonTerminal(parent);
    return false;
}

//creates a node for the parse tree
//accepts an argument of a string called name that represents the rule the node is supposed to follow
//This node will be later verified to see if it satisfies the rule given in name, and will be added to the tree if it satisfies the rule or deleted if it does not satisy the rule using other functions
Node * TopDownSyntaxAnalyzer::startNonTerminal(const string & name) {
    Node * parent = currentNode;
    currentProduction = name;
    print(name);
    currentNode = new Node(name);
    return parent;
}

//Adds a node to the parse tree
//This function is used once it is verified that a token satisfies a rule
void TopDownSyntaxAnalyzer::finishNonTerminal(Node * parent) {
    parent->add(currentNode);
    currentNode = parent;
}

//Deletes a node
//This function is used when a token fails to satisfy a rule and the node is not added to the parse tree
void TopDownSyntaxAnalyzer::cancelNonTerminal(Node * parent) {
    delete currentNode;
    currentNode = parent;
}

/*
 <ExpressionPrime> -> +<Term><ExpressionPrime> | -<Term><ExpressionPrime> | epsilon function
 A3) E' -> +TE'
 A4) E' -> epsilon
 Checks to see if a token satisfies the <ExpressionPrime> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isQ(){
    Record * record = getNextToken();
    if (record == nullptr)
        return false;
    Node * parent = startNonTerminal("<ExpressionPrime> -> +<Term><ExpressionPrime> | -<Term><ExpressionPrime> | epsilon");
    
    //A3) E' -> +TE'
    //checks for +, <Term>, <ExpressionPrime> consecutively
    //if true the corresponding rule is added to the parse tree and the function returns true
    if (record->lexeme == "+") {
        currentNode->add(new Node(*record));
        if (isT()) {
            if (isQ()) {
                print("<ExpressionPrime> -> +<Term><ExpressionPrime>");
                if (checkExpressionTypes(currentNode)) {
                    gen_instr("ADD", "nil");
                    finishNonTerminal(parent);
                    return true;
                }
            }
        }
    }
    
    //else is the program checks for -, <Term>, <ExpressionPrime> consecutively
    //if true the corresponding rule is added to the parse tree and the function returns true
    else if (record->lexeme == "-") {
        currentNode->add(new Node(*record));
        if (isT()) {
            if (isQ()) {
                print("<ExpressionPrime> -> -<Term><ExpressionPrime>");
                gen_instr("SUB", "nil")
                finishNonTerminal(parent);
                return true;
            }
        }
    }
    //else if the program checks for epsilon
    //if true the corresponding rule is added to the parse tree and the function returns true
    else if(inFollowSet(expressionPrimeFollowSet, record->lexeme)) {
        print("<ExpressionPrime> -> epsilon");
        backup();
        finishNonTerminal(parent);
        return true;
    }
    
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
    cancelNonTerminal(parent);
    return false;
}


/*
 <Term> -> <Factor><TermPrime> function
 A5) T-> FT'
 Checks to see if a token satisfies the <Term> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isT() {
    Node * parent = startNonTerminal("<Term> -> <Factor><TermPrime>");
    
    //checks for <Factor>, <TermPrime> consecutively
    //A5) T-> FT'
    //if true the node is added to the parse tree and the function returns true
    if (isF()) {
        if (isR()) {
            print("<Term> -> <Factor><TermPrime>");
            if (checkExpressionTypes(currentNode)) {
                finishNonTerminal(parent);
                return true;
            }
        }
    }
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
    cancelNonTerminal(parent);
    return false;
}

/*
 <TermPrime> -> *<Factor><TermPrime> | /<Factor><TermPrime> | epsilon function
 Checks to see if a token satisfies the <TermPrime> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isR() {
    Record * record = getNextToken();
    if (record == nullptr)
        return false;
    Node * parent = startNonTerminal("<TermPrime> -> *<Factor><TermPrime> | /<Factor><TermPrime> | epsilon");
    
    //checks for *, <Factor>, <TermPrime> consecutively
    //if true the corresponding rule is added as a node to the parse tree and the function returns true
    //A6) T' -> *FT'
    if (record->lexeme == "*") {
        currentNode->add(new Node(*record));
        if (isF()) {
            if (isR()) {
                print("<TermPrime> -> *<Factor><TermPrime>");
                finishNonTerminal(parent);
                gen_instr("MUL", "nil");
                return true;
            }
        }
    }
    //else if the function checks the second rule for /, <Factor>, <TermPrime> consecutively
    //if true the corresponding rule is added as a node to the parse tree and the function returns true
    else if(record->lexeme == "/") {
        currentNode->add(new Node(*record));
        if (isF()) {
            if (isR()) {
                print("<TermPrime> -> /<Factor><TermPrime>");
                gen_instr("DIV", "nil");
                finishNonTerminal(parent);
                return true;
            }
        }
    }
    //else if the function checks the third rule for epsilon
    //if true the corresponding rule is added as a node to the parse tree and the function returns true
    else if (inFollowSet(termPrimeFollowSet, record->lexeme)) {
        print("<TermPrime> -> epsilon");
        backup();
        finishNonTerminal(parent);
        return true;
    }
    
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
    cancelNonTerminal(parent);
    return false;
}

/*
 <Factor> -> (<Expression>) | <ID> | <NUM> function
 Checks to see if a token satisfies the <Factor> rule.
 This function uses a predictive approach using FIRST sets
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isF() {
    Record * record = getNextToken();
    if (record == nullptr)
        return false;
    backup();
    Node * parent = startNonTerminal("<Factor> -> (<Expression>) | <ID> | <NUM>");
   
    //checks for <ID> and FIRST(ID)
    //if true the corresponding rule is added as a node to the parse tree and the function returns true
    if (isId(*record) && isIdentifier(true)) {
        print("<Factor> -> <Identifier>");
        finishNonTerminal(parent);
        gen_instr("PUSHM", to_string(symbolTable.getAddress(record->lexeme)));
        return true;
    }
    
    //checks for <Number> and FIRST(Number)
    //if true the corresponding rule is added as a node to the parse tree and the function returns true
    if (isNumber(*record) && isNumberTopDown()) {
      print("<Factor> -> <NUM>");
      finishNonTerminal(parent);
      return true;
    }
    
    //checks for (, <Expression>, ), consecutively
    //if true the corresponding rule is added as a node to the parse tree and the function returns true
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
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
    cancelNonTerminal(parent);
    return false;
}

/*
 <Expression> -> <Term><ExpressionPrime> || <BooleanValue> function
 Checks to see if a token satisfies the <Expression> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isE() {
    Node * parent = startNonTerminal("<Expression> -> <Term><ExpressionPrime> || <BooleanValue>");
    Record * token = lookAhead();
    
    //checks for <BooleanValue> and FIRST(BooleanValue)
    //if true the corresponding rule is added as a node to the parse tree and the function returns true
    if (isBoolValue(*token) && isBoolValueTopDown()) {
      print("<Expression> -> <Boolean Value>");
      finishNonTerminal(parent);
      return true;
    }
    //else if checks for <Term>, <ExpressionPrime> consecutively
    //if true the corresponding rule is added as a node to the parse tree and the function returns true
    else if (isT()) {
        if (isQ()) {
            print("<Expression> -> <Term><ExpressionPrime>");
            if (checkExpressionTypes(currentNode)) {
                finishNonTerminal(parent);
                return true;
            }
            //finishNonTerminal(parent);
            //return true;
        }
    }
    
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
    cancelNonTerminal(parent);
    return false;
}

/*
 <Assign> -> <ID> = <Expression> function
 Checks to see if a token satisfies the <Assign> rule.
 Returns true if the token satisfies the rule and false if the token does not satisfy the rule
 */
bool TopDownSyntaxAnalyzer::isAssignment() {
    Node * parent = startNonTerminal("<Assign> -> <ID> = <Expression>");
    
    //checks for <ID>, =, <Expression> consecutively
    //if true the node is added to the parse tree and the function returns true
    if (isIdentifier(true)) {
        Record * record = getNextToken();
        if (record == nullptr)
            return false;
        if (record->lexeme == "=") {
            currentNode->add(new Node(*record));
            if(isE()) {
                if(checkAssignmentType(currentNode)) {
                    finishNonTerminal(parent);
                    return true;
                }
            }
        }
    }
    
    //if the function reaches this point the token does not satisfy the rule. The node is deleted using cancelNonTerminal and the function returns false to the caller
    cancelNonTerminal(parent);
    return false;
}

bool TopDownSyntaxAnalyzer::checkExpressionTypes(Node * expressionNode) {
    string expressionType;
    if (checkTypes(expressionNode, expressionType)) {
        return true;
    }

    Record token = parseTree->getLeftmostNode(expressionNode)->token;
    errorHandler.addError(Error(token, string(token.lexeme).append(" type does match the type found in other variables in this expression"), syntax_error));
    return false;
}

bool TopDownSyntaxAnalyzer::checkAssignmentType(Node *node) {
    Node * id = node->children.front()->children.front();
    Node * expression = node->children.back();
    string expressionType;
    if (checkTypes(expression, expressionType)) {
        if (symbolTable.exists(id->token.lexeme)) {
            string type = symbolTable.getType(id->token.lexeme);
            bool match = type == expressionType;
            if (!match) {
                errorHandler.addError(Error(id->token, string(id->token.lexeme).append(" type doesn't match LHS type of ").append(expressionType), syntax_error));
                return false;
            } else {
                return true;
            }
        }
    }
    errorHandler.addError(Error(id->token, string(id->token.lexeme).append(" type doesn't match LHS type of undefined"), syntax_error));
    return false;
}

bool TopDownSyntaxAnalyzer::checkTypes(Node * node, string & type) {
    if (node->children.empty()) {
        if (node->nonTerminal.empty()) {
            if (node->token.token == "IDENTIFIER") {
                if (type.empty()) {
                    type = symbolTable.getType(node->token.lexeme);
                } else {
                    return type == symbolTable.getType(node->token.lexeme);
                }
            } else if (node->token.token == "INTEGER") {
                if (type.empty()) {
                    type = "int";
                } else {
                    return type == "int";
                }
            } else if (node->token.token == "FLOAT") {
                if (type.empty()) {
                    type = "float";
                } else {
                    return type == "float";
                }
            } else if (isBoolValue(node->token)) {
                if (type.empty()) {
                    type = "bool";
                } else {
                    return type == "bool";
                }
            }
        }
        return true;
    } else {
        for (auto it = node->children.begin(); it != node->children.end(); ++it) {
            if(!checkTypes(*it, type)) {
                return false;
            }
        }
    }
    return true;
}
