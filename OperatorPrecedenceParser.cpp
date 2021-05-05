//
// Created by HashEngineering on 4/30/2021.
//

#include "OperatorPrecedenceParser.h"
OperatorTableRecord DotGreat(".>");
OperatorTableRecord LessDot("<.");
OperatorTableRecord EqualsDot("=.");
OperatorTableRecord ErrorState("ERROR","error");
OperatorTableRecord EndOfFileState("EOF","$");

OperatorTableRecord OperatorPrecedenceParser::table[6][6] = {
        // +/-
        {DotGreat, LessDot, LessDot, DotGreat, LessDot, DotGreat}, // + -
        {DotGreat, DotGreat, LessDot, DotGreat, LessDot, DotGreat}, // * /
        {LessDot,  LessDot, LessDot, EqualsDot,   LessDot, ErrorState}, // (
        {DotGreat, DotGreat, ErrorState, DotGreat, ErrorState, DotGreat}, // )
        {DotGreat, DotGreat, ErrorState, DotGreat, ErrorState, DotGreat}, // i
        {LessDot, LessDot, LessDot, ErrorState, LessDot, EndOfFileState}, // $
};

list<pair<OperatorTableRecord, string>> OperatorPrecedenceParser::productions = {
        { OperatorTableRecord("E"), "E+E"},
        { OperatorTableRecord( "E"), "E*E"},
        { OperatorTableRecord( "E"), "(E)"},
        { OperatorTableRecord( "E"), "i"}
};

OperatorPrecedenceParser::OperatorPrecedenceParser(LexicalScanner &lexicalScanner, SymbolTable &symbolTable,
                                                   ErrorHandler &errorHandler) :
    SyntaxAnalyzer(lexicalScanner, symbolTable, errorHandler) {


}

//Function to create parse tree
ParseTree * OperatorPrecedenceParser::createParseTree()
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
        errorHandler.addError({token->filename->c_str(), token->line, token->linePosition, errorMessage, syntax_error});
    }

    parseTree->printRules(cout); //print the rules of the parseTree
    return parseTree; //return parseTree to the caller
}

bool OperatorPrecedenceParser::stackProcess() {
    cout << "push " << to_string(EndOfFileState) << endl;
    productionStack.push_back(EndOfFileState);  // end of file

    Record currentToken = lexicalScanner.lexer(); //get first token
    cout << currentToken << endl;

    currentNode = parseTree->getRoot();
    parseTreeStack.push(currentNode);
    Node * child = currentNode->add(new Node(to_string(productionStack.back())));
    parseTreeStack.push(child);


    while (!productionStack.empty()) {
        cout << "stack: ";// << to_string(productionStack.back()) << endl;
        auto xt = productionStack.back();
        for (auto i : productionStack) {
            cout << i.value;
        }
        cout << endl;
        auto x = getTopMostTerminal();
        currentNode = parseTreeStack.top();
        cout << "top of stack (terminal): " << to_string(x) << endl;
        //cout << "top of nodes: " << currentNode->nonTerminal << endl;

        if (isEOF(currentToken) && x.value == "$")
            break;

        OperatorTableRecord cell = table[columnFromRecord(x)][columnFromToken(currentToken)];

        if (cell == LessDot || cell == EqualsDot) {
            if (x.value != xt.value) {
                productionStack.push_back(cell);
                productionStack.push_back(xt);
                productionStack.push_back(OperatorTableRecord(currentToken));
                currentToken = lexicalScanner.lexer();
            } else {
                productionStack.push_back(cell);
                productionStack.push_back(OperatorTableRecord(currentToken));
                currentToken = lexicalScanner.lexer();
            }
        } else if (cell == DotGreat) {
            list<OperatorTableRecord> productionToReduce;
            if (x.value != xt.value) {
                productionStack.push_back(xt);
            }
            while (productionStack.back() != LessDot) {
                productionToReduce.push_front(productionStack.back());
                productionStack.pop_back();
            }

            string newRule;
            for (auto it : productionToReduce) {
                newRule.append(it.value);
            }
            productionStack.pop_back();

            OperatorTableRecord rule = ErrorState;
            cout << "try to match this rule:" << newRule << endl;
            for (auto it: productions) {
                if (newRule == it.second) {
                    rule = it.first;
                    break;
                }
            }
            if (rule == ErrorState) {
                return false;
            }
            productionStack.push_back(rule);
        } else if (cell == ErrorState) {
            return false;
        }
    }
    return true;
}

OperatorTableRecord OperatorPrecedenceParser::getTopMostTerminal() {
    if (isTerminal(productionStack.back()))
        return productionStack.back();

    while(!isTerminal(productionStack.back())) {
        productionStack.pop_back();
    }
    return productionStack.back();
    /*list<OperatorTableRecord>::reverse_iterator rit = productionStack.rend();
    while (rit != productionStack.rbegin()) {
        rit--;
        if (isTerminal(*rit))
            return *rit;
    }*/
}

int OperatorPrecedenceParser::columnFromRecord(OperatorTableRecord & record) {
    if (record.token.token == "") {
        Record token = Record("", record.value, true, "", 0, 0, "");
        return columnFromToken(token);
    }
    return columnFromToken(record.token);
}

int OperatorPrecedenceParser::columnFromToken(Record & token) {
    if(isId(token))
        return Identifier;

    if(isPlus(token)) {
        return Addition;
    }

    if(isMinus(token)) {
        return Subtraction;
    }

    if(isMultiply(token)) {
        return Multiply;
    }

    if(isDivideBy(token)) {
        return Divide;
    }

    if(isLeftParen(token)) {
        return LeftParen;
    }

    if(isRightParan(token)) {
        return RightParen;
    }

    if(isEOF(token)) {
        return EndOfFile;
    }

    return Error;
}