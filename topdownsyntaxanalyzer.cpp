#include "topdownsyntaxanalyzer.h"
#include "parsetree.h"
#include <iostream>
using namespace std;

ParseTree * TopDownSyntaxAnalyzer::createParseTree() {

    ParseTree* result = new ParseTree();

    while(isStatement()) {
        cout << "processed statement successfully" << endl;
    }

    /*list<Record> statement = getNextStatement();
    while (!statement.empty()) {
        cout << "parsing " << *statement.begin() << endl;
        isStatement(statement);
        statement = getNextStatement();
    }*/

    return result;
}

bool TopDownSyntaxAnalyzer::isStatement() {
    int it = currentLexeme;
    if(isDeclaration())
        return true;

    currentLexeme = it;
    //  
    if(isAssignment()) {
        print("<Statement> -> <Assign>");  
        return true;
    }

    return false;
}

bool TopDownSyntaxAnalyzer::isDeclaration() {

    Record * record = getNextToken();
    Record * type = record;
    if (record == nullptr) 
        return false;
    print("<Declaration> -> <Type><ID>");
    
    if (isType(*record)) {
        Record * record = getNextToken();
        Record * id = record;
        if (record == nullptr) 
            return false;

        if (isId(*record)) {
            Record * record = getNextToken();
            if (record == nullptr) 
                return false;
            if (record->lexeme == ";") {
                //cout << *type << endl;
                //cout << *id << endl;
                //cout << *record << endl;
                print("<Declaration> -> <Type><ID>");
                return true;
            }
        } else {

        }
    }
    return false;
}

bool TopDownSyntaxAnalyzer::isExpression(list<Record> & statement) {
    return false;
}

bool TopDownSyntaxAnalyzer::isTerm(list<Record> & statement, list<Record>::iterator & it) {
    /*list<Record>::iterator = statement.find()
    if (isFactor(statement, it)) {
        if (isR)
    }*/
    return false;
}
/*
bool TopDownSyntaxAnalyzer::isFactor(list<Record> & statement, list<Record>::iterator & it) {
    
    if (it->token == "SEPARATOR" && it->lexeme == "(") {
        // advance to )
        list<Record>::iterator itTemp = it;
        list<Record> expression;
        bool foundRightParenthesis = false;
        while (itTemp != statement.end()) {
            if (itTemp->lexeme == ")") {
                foundRightParenthesis = true;
            } else {
                expression.push_back(*itTemp);
                itTemp++;
            }
        }
        if (foundRightParenthesis) {
            it = ++itTemp;
            cout << "<Factor> -> (<Expression>)" << endl;
            return true;
        } else {
            return false;
        }
    } else if (isId(*it)) {
        print("<Factor> -> <ID>");
        return true;
    } else if (isNumber(*it)) {
        print("<Factor> -> <Num>");
        return true;
    }
    return false;
}*/

bool TopDownSyntaxAnalyzer::isQ(){
    Record * record = getNextToken();
    if (record == nullptr) 
        return false;

    if (record->lexeme == "+") {
        if (isT()) {
            if (isQ()) {
                //cout << " Q -> +TQ" << endl;
                print("<ExpressionPrime> -> +<Term><ExpressionPrime>");
                return true;
            }
        } 
    } else if (record->lexeme == ")" || record->lexeme == ";") {
        backup();
        //cout << " Q -> epsilon" << endl;
        print("<ExpressionPrime> -> epsilon");
        return true;
    }
    return false;
}

bool TopDownSyntaxAnalyzer::isT() {
    //print(" <Term> -> <Factor><TermPrime>");
    if (isF()) {
        if (isR()) {
            //cout << " T -> FR" << endl;
            print("<Term> -> <Factor><TermPrime>");
            return true;
        }
    }
    return true;
}

bool TopDownSyntaxAnalyzer::isR() {
    Record * record = getNextToken();
    if (record == nullptr) 
        return false;

    if (record->lexeme == "*") {
        if (isF()) {
            if (isR()) {
                //cout << *record << endl;
                //cout << " R -> *FR" << endl;
                print("<TermPrime> -> *<Factor><TermPrime>");
                return true;
            }
        }
    } else if (record->lexeme == "+" || record->lexeme == ")" || record->lexeme == ";") {
        //cout << *record << endl;
        print("<TermPrime> -> epsilon");
        backup();
        return true;
    }
    return false;
}

bool TopDownSyntaxAnalyzer::isF() {
    Record * record = getNextToken();
    if (record == nullptr) 
        return false;
    //print(" <Factor> -> <Identifier>");
    if (isId(*record)) {
        //cout << *record << endl;
        //cout << " F -> id" << endl;
        print("<Factor> -> <Identifier>");
        return true;
    } else {
        //print(" <Factor> -> (<Expression>)");
        if (record->lexeme == "(") {
            if (isE()) {
                Record * record = getNextToken();
                if (record == nullptr) 
                    return false;
                
                if (record->lexeme == ")") {
                    //cout << " F -> (E)" << endl;
                    print("<Factor> -> (<Expression>)");
                    return true;
                }
            }
        }
    }
    return false;
}

bool TopDownSyntaxAnalyzer::isE() {
    //print(" <Expression> -> <Term><ExpressionPrime>");
    //cout << *currentLexeme << endl;
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
            return true;
        }
    }
/**
 * Determines if the next token is an identifier
 */
bool TopDownSyntaxAnalyzer::isIdentifier() {
    Record * record = getNextToken();
    if (record == nullptr) 
        return false;
    if(isId(*record)) {
        currentNode->add(new Node(*record));
        return true;
    }
    return false;
}

bool TopDownSyntaxAnalyzer::isAssignment() {
    //cout << *currentLexeme << endl;
    //print(" <Assign> -> <ID> = <Expression>");
    if (isIdentifier()) {
        Record * record = getNextToken();
        if (record == nullptr) 
            return false;
        //cout << * record << endl;
        if (record->lexeme == "=") {
            if(isE()) {
                Record * record = getNextToken();
                if (record == nullptr) 
                    return false;
                cout << * record << endl;
                // the ; optionally ends a statement
                if (record->lexeme != ";")
                    backup();

                //cout << " <Assign> -> <ID> = <Expression>;" << endl;
                print("<Assign> -> <ID> = <Expression>");
                return true;
            }
        }
    }
    return false;
}