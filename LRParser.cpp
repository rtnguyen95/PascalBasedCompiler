//
// Created by HashEngineering on 4/30/2021.
//

#include "LRParser.h"
#include "astparsetree.h"
LREntry StateZero(LREntry::Goto, 0);
// Error/Empty
LREntry _E(LREntry::Error);
LREntry AC(LREntry::Accept);
LREntry EndOfFileEntry(LREntry::EndOfFile);
ShiftEntry S3(3);
ShiftEntry S4(4);
ShiftEntry S5(5);
ShiftEntry S6(6);
ShiftEntry S7(7);
ShiftEntry S8(8);
ShiftEntry S9(9);
ShiftEntry S15(15);
ReduceEntry R1(1);
ReduceEntry R2(2);
ReduceEntry R3(3);
ReduceEntry R4(4);
ReduceEntry R5(5);
ReduceEntry R6(6);
ReduceEntry R7(7);
ReduceEntry R8(8);
GotoEntry G1(1);
GotoEntry G2(2);
GotoEntry G3(3);
GotoEntry G10(10);
GotoEntry G11(11);
GotoEntry G12(12);
GotoEntry G13(13);
GotoEntry G14(14);
LREntry LRParser::table[16][11] = {
        //id,  +,  -,  *,  /,  (,  ),  $,  E,  T,  F
        { S5, _E, _E, _E, _E, S4, _E, _E, G1, G2, G3 }, // 0
        { _E, S6, S7, _E, _E, _E, _E, AC, _E, _E, _E }, // 1
        { _E, R3, R3, S8, S9, _E, R3, R3, _E, _E, _E }, // 2
        { _E, R6, R6, R6, R6, _E, R6, R6, _E, _E, _E }, // 3
        { S5, _E, _E, _E, _E, S4, _E, _E,G10, G2, G3 }, // 4
        { _E, R8, R8, R8, R8, _E, R8, R8, _E, _E, _E }, // 5
        { S5, _E, _E, _E, _E, S4, _E, _E, _E,G11, G3 }, // 6
        { S5, _E, _E, _E, _E, S4, _E, _E, _E,G12, G3 }, // 7
        { S5, _E, _E, _E, _E, S4, _E, _E, _E, _E, G13}, // 8
        { S5, _E, _E, _E, _E, S4, _E, _E, _E, _E, G14}, // 9
        { _E, S6, S7, _E, _E, _E,S15, _E, _E, _E, _E }, // 10
        { _E, R1, R1, S8, S9, _E, R1, R1, _E, _E, _E }, // 11
        { _E, R2, R2, S8, S9, _E, R2, R2, _E, _E, _E }, // 12
        { _E, R4, R4, R4, R4, _E, R4, R4, _E, _E, _E }, // 13
        { _E, R5, R5, R5, R5, _E, R5, R5, _E, _E, _E }, // 14
        { _E, R7, R7, R7, R7, _E, R7, R7, _E, _E, _E }, // 15
};

list<pair<LREntry, string>> LRParser::productions = {
        { LREntry("E", 1), "E+T"},
        { LREntry("E", 2), "E-T"},
        { LREntry("E", 3), "T"},
        { LREntry("T", 4), "T*F"},
        { LREntry("T", 5), "T/F"},
        { LREntry("T", 6), "F"},
        { LREntry("F", 7), "(E)"},
        { LREntry("F", 8), "i"},
};

LRParser::LRParser(LexicalScanner &lexicalScanner, SymbolTable &symbolTable,
                                                   ErrorHandler &errorHandler) :
    SyntaxAnalyzer(lexicalScanner, symbolTable, errorHandler) {


}

//Function to create parse tree
ParseTree * LRParser::createParseTree()
{
    parseTree = new ASTParseTree(); //initialize the parse tree
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
            cout << "the stack " << currentProduction << " could not be reduced with " << token->lexeme << endl;

            //add information about the rule and the lexeme that mismatched to errorMessage
            errorMessage.append("this stack ").append(currentProduction).append(" could not be reduced with '").append(token->lexeme)
                    .append("'\n").append(lastError);
        }

        //pass the file name, line number, line position number, error message string, and syntax_error information to errorHandler.addError so it can be recorded
        errorHandler.addError({token->filename->c_str(), token->line, token->linePosition, errorMessage, syntax_error});
    }

    //parseTree->printRules(cout); //print the rules of the parseTree
    parseTree->printTree(cout);
    printIntermediateCode(cout);
    return parseTree; //return parseTree to the caller
}

//function to return the stack in the LR parser as a string
string LRParser::getStackAsString() {
    string result;
    for (auto i : productionStack) {
        result.append(to_string(i));
    }
    return result;
}



bool LRParser::stackProcess() {
    cout << "push " << to_string(EndOfFileEntry) << endl;
    productionStack.push_back(EndOfFileEntry);  // end of file

    cout << "push " << to_string(StateZero) << endl;
    productionStack.push_back(StateZero);  // end of file

    Record currentToken = *getNextToken(); //get first token
    cout << currentToken << endl;

    currentNode = parseTree->getRoot();
    parseTreeStack.push(currentNode);


    while (!productionStack.empty()) {
        cout << "--------------------------------------------------" << endl;
        cout << "stack: ";
        cout << getStackAsString();
        cout << endl;
        auto qm = productionStack.back();
        //currentNode = parseTreeStack.top();
        cout << "top of stack: " << to_string(qm) << " and current token: " << currentToken.lexeme << endl;
        //cout << "top of nodes: " << currentNode->nonTerminal << endl;

        int row = rowFromRecord(qm);
        int column = columnFromToken(currentToken);
        if (column == Unknown) {
            errorHandler.addError(Error(currentToken, "unknown token", syntax_error));
        }
        cout << "T[" << to_string(qm) << ", " << to_string(LREntry(currentToken)) << "] = ";
        LREntry x = table[row][column];
        cout << to_string(x) << "; Action -> ";
        switch(x.operation) {
            case LREntry::Shift:
                cout << "Shift( " << x.value << "); push(" << to_string(LREntry(currentToken)) << "); push (" << to_string(GotoEntry(x.value)) << ")" << endl;
                productionStack.push_back(LREntry(currentToken));
                productionStack.push_back(GotoEntry(x.value));
                if (isId(currentToken)) {
                    parseTreeStack.push(new Node(currentToken));
                    if(!symbolTable.exists(currentToken.lexeme))
                        symbolTable.add(Record(string("KEYWORD"), string("int"), true, *currentToken.filename, currentToken.line, currentToken.linePosition), currentToken);
                }
                currentToken = *getNextToken();
                break;
            case LREntry::Reduce: {
                cout << "Reduce(" << x.value << "): ";
                // pop 2x RHS symbols
                int n = getProductionTerminalCount(x.value) * 2;
                while (n--) {
                    cout << "pop("<<to_string(productionStack.back())<<");";
                    auto top = productionStack.back();
                    //add to the parse tree
                    if (isOperator(top.token)) {
                        Node * op2 = parseTreeStack.top();
                        parseTreeStack.pop();
                        Node * op1 = parseTreeStack.top();
                        parseTreeStack.pop();
                        currentNode = new Node(top.token);
                        currentNode->nonTerminal = getProduction(x.value).production + " -> " + getProductionRHS(x.value);
                        currentNode->add(op1);
                        currentNode->add(op2);
                        parseTreeStack.push(currentNode);
                    }
                    productionStack.pop_back();
                }
                // let qj be the TOS
                auto qj = productionStack.back();
                // push LHS onto the stack
                auto lhs = getProduction(x.value);
                cout << " Production: " << to_string(lhs) << " -> " << getProductionRHS(x.value) << "; ";
                productionStack.push_back(lhs);
                int row = rowFromRecord(qj);
                int column = columnFromRecord(lhs);

                cout << " T[" << to_string(qj) << ", " << to_string(lhs) << "]; ";
                auto qk = table[row][column];
                cout << "push(" << to_string(qk) << ")" << endl;
                productionStack.push_back(qk);
                break;
            }
            case LREntry::Accept:
                parseTree->getRoot()->add(parseTreeStack.top());
                return true;
            default:
                currentProduction = getStackAsString();
                errorHandler.addError(Error(currentToken,
                                            string("LR stack[").append(getStackAsString()).append("] gives invalid action for ")
                                            .append("T[").append(to_string(qm)).append(", ").append(to_string(LREntry(currentToken))).append("]"),
                                            syntax_error));
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
    //auto column = productionNames[record.production];

    //return column;
    if (record.production == "E") {
        return EndOfFile + 1;
    } else if (record.production == "T") {
        return EndOfFile + 2;
    } else if (record.production == "F") {
        return EndOfFile + 3;
    }
    throw bad_exception();
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

    return Unknown;
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

string LRParser::getProductionRHS(int rule) {
    for (auto i : productions) {
        if (i.first.value == rule) {
            return i.second;
        }
    }
    return 0;
}


// use LRV
void LRParser::printIntermediateCode(ostream & stream) {
    stream << "---------------Intermediate Code -------------------" << endl;
    printIntermediateCode(stream, parseTree->getRoot());
    for (int i = 0; i < instr_table.size(); ++i) {
        auto it = instr_table[i];
        stream << it.op << " " << it.oprnd << endl;
    }
}

void LRParser::printIntermediateCode(ostream & stream, Node * node) {
    for (auto & it : node->children) {
        printIntermediateCode(stream, it);
    }
    if (node->token.lexeme.empty()) {
        //print nothing for the root node
    } else if(node->token.token == "IDENTIFIER") {
        gen_instr("PUSHM", ::to_string(symbolTable.getAddress(node->token.lexeme)));
    } else if(isPlus(node->token)) {
        gen_instr("ADD", "");
    } else if(isMultiply(node->token)) {
        gen_instr("MUL", "");
    } else if(isDivideBy(node->token)) {
        gen_instr("DIV", "");
    } else if(isMinus(node->token)) {
        gen_instr("SUB", "");
    }
}
