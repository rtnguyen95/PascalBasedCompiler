//
// Created by HashEngineering on 4/30/2021.
//

#include "LRParser.h"
LREntry StateZero(LREntry::Goto, 0);
LREntry ErrorEntry(LREntry::Error);
LREntry AcceptEntry(LREntry::Accept);
LREntry EndOfFileEntry(LREntry::EndOfFile);

LREntry LRParser::table[12][9] = {
        // +/-
        { ShiftEntry(5), ErrorEntry, ErrorEntry, ShiftEntry(4), ErrorEntry, ErrorEntry, GotoEntry(1), GotoEntry(2), GotoEntry(3), },
        { ErrorEntry, ShiftEntry(6), ErrorEntry, ErrorEntry, ErrorEntry, AcceptEntry, ErrorEntry, ErrorEntry, ErrorEntry},
        { ErrorEntry, ReduceEntry(2), ShiftEntry(7), ErrorEntry, ReduceEntry(2), ReduceEntry(2), ErrorEntry, ErrorEntry, ErrorEntry},
        { ErrorEntry, ReduceEntry(4), ReduceEntry(4), ErrorEntry, ReduceEntry(4), ReduceEntry(4), ErrorEntry, ErrorEntry, ErrorEntry},
        { ShiftEntry(5), ErrorEntry, ErrorEntry, ShiftEntry(4), ErrorEntry, ErrorEntry, GotoEntry(8), GotoEntry(2), GotoEntry(3)},
        { ErrorEntry, ReduceEntry(6), ReduceEntry(6), ErrorEntry, ReduceEntry(6), ReduceEntry(6),ErrorEntry, ErrorEntry, ErrorEntry},
        { ShiftEntry(5), ErrorEntry, ErrorEntry, ShiftEntry(4), ErrorEntry, ErrorEntry, ErrorEntry, GotoEntry(9), GotoEntry(3)},
        { ShiftEntry(5), ErrorEntry, ErrorEntry, ShiftEntry(4), ErrorEntry, ErrorEntry, ErrorEntry, ErrorEntry, GotoEntry(10)},
        { ErrorEntry, ShiftEntry(6), ErrorEntry, ErrorEntry, ShiftEntry(11), ErrorEntry, ErrorEntry, ErrorEntry, ErrorEntry},
        { ErrorEntry, ReduceEntry(1), ShiftEntry(7), ErrorEntry, ReduceEntry(1), ReduceEntry(1), ErrorEntry, ErrorEntry, ErrorEntry},
        { ErrorEntry, ReduceEntry(3), ReduceEntry(3), ErrorEntry, ReduceEntry(3), ReduceEntry(3), ErrorEntry, ErrorEntry, ErrorEntry},
        { ErrorEntry, ReduceEntry(5), ReduceEntry(5), ErrorEntry, ReduceEntry(5), ReduceEntry(5), ErrorEntry, ErrorEntry, ErrorEntry},

};

list<pair<LREntry, string>> LRParser::productions = {
        { LREntry("E", 1), "E+T"},
        { LREntry("E", 2), "T"},
        { LREntry("T", 3), "T*F"},
        { LREntry("T", 4), "F"},
        { LREntry("F", 5), "(E)"},
        { LREntry("F", 6), "i"},
};

LRParser::LRParser(LexicalScanner &lexicalScanner, SymbolTable &symbolTable,
                                                   ErrorHandler &errorHandler) :
    SyntaxAnalyzer(lexicalScanner, symbolTable, errorHandler) {


}

//Function to create parse tree
ParseTree * LRParser::createParseTree()
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

bool LRParser::stackProcess() {
    cout << "push " << to_string(EndOfFileEntry) << endl;
    productionStack.push_back(EndOfFileEntry);  // end of file

    cout << "push " << to_string(StateZero) << endl;
    productionStack.push_back(StateZero);  // end of file

    Record currentToken = lexicalScanner.lexer(); //get first token
    cout << currentToken << endl;

    currentNode = parseTree->getRoot();
    parseTreeStack.push(currentNode);
    Node * child = currentNode->add(new Node(to_string(productionStack.back())));
    parseTreeStack.push(child);


    while (!productionStack.empty()) {
        cout << "stack: ";// << to_string(productionStack.back()) << endl;
        for (auto i : productionStack) {
            cout << to_string(i);
        }
        cout << endl;
        auto qm = productionStack.back();
        //currentNode = parseTreeStack.top();
        cout << "top of stack: " << to_string(qm) << endl;
        //cout << "top of nodes: " << currentNode->nonTerminal << endl;

        int row = rowFromRecord(qm);
        int column = columnFromToken(currentToken);
        LREntry x = table[row][column];

        switch(x.operation) {
            case LREntry::Shift:
                cout << "Shift: " << "push(" << to_string(LREntry(currentToken)) << "); push (" << to_string(GotoEntry(x.value)) << ")" << endl;
                productionStack.push_back(LREntry(currentToken));
                productionStack.push_back(GotoEntry(x.value));
                currentToken = lexicalScanner.lexer();
                break;
            case LREntry::Reduce: {
                cout << "Reduce: " << x.value << " ";
                // pop 2x RHS symbols
                int n = getProductionTerminalCount(x.value) * 2;
                while (n--) {
                    cout << "pop("<<to_string(productionStack.back())<<");";
                    productionStack.pop_back();
                }
                cout << endl;
                // let qj be the TOS
                auto qj = productionStack.back();
                // push LHS onto the stack
                auto lhs = getProduction(x.value);
                productionStack.push_back(lhs);
                int row = rowFromRecord(qj);
                int column = columnFromRecord(lhs);
                auto qk = table[row][column];
                productionStack.push_back(qk);
                break;
            }
            case LREntry::Accept:
                return true;
            default:
                return false;
        }
    }
    return false;
}

int LRParser::rowFromRecord(LREntry & record) {
    return record.value;
}

int LRParser::columnFromRecord(LREntry & record) {
    if (!productionNames.count(record.production)) {
        throw bad_exception();
    }
    auto column = productionNames[record.production];

    return column;
}

int LRParser::columnFromToken(Record & token) {
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

    throw bad_exception();
}

int LRParser::getProductionTerminalCount(int rule) {
    for (auto i : productions) {
        if (i.first.value == rule) {
            return i.second.length();
        }
    }
    return 0;
}

LREntry LRParser::getProduction(int rule) {
    for (auto i : productions) {
        if (i.first.value == rule) {
            return i.first;
        }
    }
    throw bad_exception();
}
