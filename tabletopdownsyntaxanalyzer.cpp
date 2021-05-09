#include "tabletopdownsyntaxanalyzer.h"

TableTopDownSyntaxAnalyzer::TableTopDownSyntaxAnalyzer(LexicalScanner & lexicalScanner, SymbolTable & symbolTable, ErrorHandler & errorHandler) 
: SyntaxAnalyzer(lexicalScanner, symbolTable, errorHandler) {
    
    table[Expression] = {
        {Identifier, {Term, ExpressionPrime}},
        {LeftParenthesis, {Term, ExpressionPrime}}
    };

    table[ExpressionPrime] = {
        {Addition, {Addition, Term, ExpressionPrime}},
        {Subtraction, {Subtraction, Term, ExpressionPrime}},
        {RightParenthesis, {Epsilon}},
        {EndofFile, {Epsilon}},
        {Semicolon, {Epsilon}},
    };

    table[Term] = {
        {Identifier, {Factor, TermPrime}},
        {LeftParenthesis, {Factor, TermPrime}},
    };

    table[TermPrime] = {
        {Addition, {Epsilon}},
        {Subtraction, {Epsilon}},
        {Multiplication, {Multiplication, Factor, TermPrime}},
        {Division, {Division, Factor, TermPrime}},
        {RightParenthesis, {Epsilon}},
        {EndofFile, {Epsilon}},
        {Semicolon, {Epsilon}},
    };

    table[Factor] = {
        {Identifier, {Identifier}},
        {LeftParenthesis, {LeftParenthesis, Expression, RightParenthesis}}
    };

    table[Assignment] = {
            {Identifier, {Identifier, Equals, Expression}}
    };

    table[Declarative] = {
            { Type, {Type, Identifier}}
    };
    table[Statement] = {
            { Identifier, {Assignment}},
            { Type, {Declarative}}
    };
    table[StatementList] = {
            { Identifier, {Statement, MoreStatements}},
            { Type, {Statement, MoreStatements}}
    };
    table[MoreStatements] = {
            {Semicolon, { Semicolon, Statement, MoreStatements}},
            {EndofFile, { Epsilon }}
    };

    table[Conditional] = {
            {Identifier, {Expression, RelativeOperator, Expression}},
            {LeftParenthesis, {Expression}}
    };
}


//Function to create parse tree
ParseTree * TableTopDownSyntaxAnalyzer::createParseTree()
{
    parseTree = new ParseTree(); //initialize the parse tree
    currentNode = parseTree->getRoot(); // always add statements to the root node
    
    //create a boolean variable called result and assign it to the outcome of isStatementList. result will return true if processing is successful and false if processing fails
    bool result = stackProcess();
    
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
        errorHandler.addError({*token->filename, token->line, token->linePosition, errorMessage, syntax_error});
    }
    
    parseTree->printRules(cout); //print the rules of the parseTree
    return parseTree; //return parseTree to the caller
}

bool TableTopDownSyntaxAnalyzer::stackProcess() {
    cout << "push " << to_string(EndofFile) << endl;
    productionStack.push(EndofFile);  // end of file
    
    cout << "push " << to_string(StatementList) << endl;
    productionStack.push(StatementList); // first symbol
    
    Record currentToken = * getNextToken(); //get first token
    cout << currentToken << endl;

    currentNode = parseTree->getRoot();
    parseTreeStack.push(currentNode);
    Node * child = currentNode->add(new Node(to_string(productionStack.top())));
    parseTreeStack.push(child);


    while (!productionStack.empty()) {
        auto x = productionStack.top();
        currentNode = parseTreeStack.top();
        cout << "top of stack: " << to_string(x) << endl;
        //cout << "top of nodes: " << currentNode->nonTerminal << endl;

        if (isTerminal(x)) {
            if (x == columnFromToken(currentToken)) {
                currentNode->token = currentToken;
                currentNode->nonTerminal = "";
                
                cout << "pop " << to_string(productionStack.top()) << endl;
                productionStack.pop();
                parseTreeStack.pop();

                if (isId(currentToken) && currentNode->parent->nonTerminal == productions[Declarative]) {
                    if (symbolTable.exists(currentToken.lexeme)) {
                        errorHandler.addError(Error(currentToken, string("identifier was already declared."), syntax_error));
                    } else {
                        Node * typeNode = currentNode->parent->children.front();
                        symbolTable.add(typeNode->token, currentToken);
                    }
                }
                
                // obtain the next token
                currentToken = *getNextToken();
                cout << currentToken << endl;
            } else {
                cout << "Terminal not found by columnFromToken" << endl;
                return false;
            }
        } else {
            int a = columnFromToken(currentToken);

            // check for the row in the table
            if (table.find((Productions)x) != table.end()) {
                auto row = table[(Productions)x];
                // check for the column a in the row
                if (row.find((Terminals)a) != row.end()) {
                    auto cell = row[(Terminals)a];

                    // pop the current production rule
                    cout << "pop " << to_string(productionStack.top()) << endl;
                    productionStack.pop();
                    currentNode = parseTreeStack.top();
                    parseTreeStack.pop();
                    currentProduction = productions[x];

                    // push the cell onto the stack in reverse order
                    // also create nodes in the parse tree
                    for (auto it = cell.rbegin(); it != cell.rend(); it++) {
                        // if this production is not epsilon
                        if (*it != Epsilon) {
                            productionStack.push(*it);
                            if (!isTerminal(*it)) {
                                Node * child = currentNode->addFirst(new Node(productions[*it]));
                                parseTreeStack.push(child);
                                cout << "push " << to_string(*it) << endl;
                            } else {
                                Node * child = currentNode->addFirst(new Node(to_string(Terminal))); 
                                parseTreeStack.push(child);
                                cout << "push " << to_string(*it) << endl;
                            }
                        }
                    }
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
    }
    return true;
}


int TableTopDownSyntaxAnalyzer::columnFromToken(Record & token) {
    if(isId(token))
        return Identifier;
    
    if(isRelativeOperator(token))
        return RelativeOperator;
    
    if(isPlus(token)) {
        return Addition;
    }
    
    if(isMinus(token)) {
        return Subtraction;
    }
        
    if(isMultiply(token)) {
        return Multiplication;
    }
        
    if(isDivideBy(token)) {
        return Division;
    }

    if(isLeftParen(token)) {
        return LeftParenthesis;
    }

    if(isRightParan(token)) {
        return RightParenthesis;
    }

    if(isEOF(token)) {
        return EndofFile;
    }

    if(isEquals(token)) {
        return Equals;
    }

    if (isType(token)) {
        return Type;
    }

    if(isSemiColon(token)) {
        return Semicolon;
    }

    return ErrorState;
}