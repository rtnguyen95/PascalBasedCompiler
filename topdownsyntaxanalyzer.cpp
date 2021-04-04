#include "topdownsyntaxanalyzer.h"
#include "parsetree.h"
#include <iostream>
using namespace std;

ParseTree * TopDownSyntaxAnalyzer::createParseTree() {

    parseTree = new ParseTree();

    bool hasErrors = false;
    while(!hasErrors) {
        currentNode = parseTree->getRoot(); // always add statements to the root node
        if (lexicalScanner.isFinished())
            break;

        hasErrors = !isStatement();
        if (!hasErrors)
            cout << "processed statement successfully" << endl;
        else cout << "processing statement with failure" << endl;
    }

    cout << "-------Parse Tree Code------------------" << endl;
    parseTree->printNodes(false);
    cout << "-------Parse Tree Rules------------------" << endl;
    parseTree->printRules();
    cout << "-------Parse Tree -----------------------" << endl;
    parseTree->printTree();
    return parseTree;
}

bool TopDownSyntaxAnalyzer::isStatement() {
    int it = currentLexeme;
    Node * currentNode = parseTree->getRoot();
    Node * parent = startNonTerminal("<Statement> -> <Assign> | <Declaration>");
    if(isDeclaration()) {
        finishNonTerminal(parent);
        return true;
    }

    currentLexeme = it;
    //
    if(isAssignment()) {
        //print("<Statement> -> <Assign>");
        finishNonTerminal(parent);
        return true;
    }

    cancelNonTerminal(parent);
    return false;
}

bool TopDownSyntaxAnalyzer::isDeclaration() {

    // Record * record = getNextToken();
    // if (record == nullptr)
    //     return false;
    Record * record = getNextToken();
    if (record == nullptr) {return false;}
    Node * declarationNode = new Node("Declaration");
    Node * parent = startNonTerminal("Type");
    if (isType(*record)) {
      print("<Declaration> -> <Type><ID>");
      print("<Type> -> float | int | bool");
      currentNode->add(new Node(*record));
      finishNonTerminal(parent);
      declarationNode->add(parent);
      Record type = *record;

      Record * record = getNextToken();
      if (record == nullptr) {return false;}
      Node * parent = startNonTerminal("ID");
      if(isId(*record)) {
          currentNode->add(new Node(record));
          finishNonTerminal(parent);
          declarationNode->add(parent);
          Record id = *record;
          Record * record = getNextToken();
        if (record == nullptr)
          return false;
        if (record->lexeme == ";") {
            //cout << *type << endl;
            //cout << *id << endl;
            //cout << *record << endl;
            // Node * declarationNode = new Node("Declaration");
            // currentNode->add(declarationNode);
            //
            // declarationNode->add(new Node(type));
            // declarationNode->add(new Node(id));
            // declarationNode->add(new Node(*record));
          print("<Declaration> -> <Type><ID>");
          return true;
        }
        else {return false;}
      }
      else {
        cancelNonTerminal(parent);
        return false;
      }
    }
    cancelNonTerminal(parent);
    return false;

    // if (isTypeTopDown()) {
    //     Record * record = getNextToken();
    //     Record type = *record;
    //     print("<Declaration> -> <Type><ID>");
    //     print("<Type> -> float | int | bool");
    //     if (record == nullptr)
    //         return false;
    //     Record id = *record;
    //     if (isId(*record)) {
    //         Record * record = getNextToken();
    //         if (record == nullptr)
    //             return false;
    //         if (record->lexeme == ";") {
    //             //cout << *type << endl;
    //             //cout << *id << endl;
    //             //cout << *record << endl;
    //             // Node * declarationNode = new Node("Declaration");
    //             // currentNode->add(declarationNode);
    //             //
    //             // declarationNode->add(new Node(type));
    //             // declarationNode->add(new Node(id));
    //             // declarationNode->add(new Node(*record));
    //             print("<Declaration> -> <Type><ID>");
    //             return true;
    //         }
    //     } else {
    //
    //     }
    // }
    // return false;
}
bool TopDownSyntaxAnalyzer::isTypeTopDown() {
  Record * record = getNextToken();
  Node * parent = startNonTerminal("Type");
  if (isType(*record)) {
    currentNode->add(new Node(*record));
    finishNonTerminal(parent);
    return true;
  }
  cancelNonTerminal(parent);
  return false;
}

Node * TopDownSyntaxAnalyzer::startNonTerminal(const string & name) {
    Node * parent = currentNode;
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
    } else if (record->lexeme == ")" || record->lexeme == ";") {
        backup();
        //cout << " Q -> epsilon" << endl;
        print("<ExpressionPrime> -> epsilon");
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
    } else if (record->lexeme == "+" || record->lexeme == "-" || record->lexeme == ")" || record->lexeme == ";") {
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
    Node * parent = startNonTerminal("<Factor> -> (<Expression>) | <ID>");
    //print(" <Factor> -> <Identifier>");
    if (isId(*record)) {
        //cout << *record << endl;
        //cout << " F -> id" << endl;
        print("<Factor> -> <Identifier>");
        currentNode->add(new Node(*record));
        finishNonTerminal(parent);
        return true;
    } else {
        //print(" <Factor> -> (<Expression>)");
        if (record->lexeme == "(") {
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
    }
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

/**
 * Determines if the next token is an identifier
 */
bool TopDownSyntaxAnalyzer::isIdentifier() {
    Record * record = getNextToken();
    if (record == nullptr)
        return false;
    if(isId(*record)) {
        currentNode->add(new Node(record));
        return true;
    }
    return false;
}

bool TopDownSyntaxAnalyzer::isAssignment() {
    //cout << *currentLexeme << endl;
    //print(" <Assign> -> <ID> = <Expression>");
    Node * parent = startNonTerminal("<Assign> -> <ID> = <Expression>");
    if (isIdentifier()) {
        Record * record = getNextToken();
        if (record == nullptr)
            return false;
        //cout << * record << endl;
        if (record->lexeme == "=") {
            currentNode->add(new Node(*record));
            if(isE()) {
                Record * record = getNextToken();
                if (record == nullptr)
                    return false;
                cout << * record << endl;
                // the ; optionally ends a statement
                if (record->lexeme != ";")
                    backup();

                //cout << " <Assign> -> <ID> = <Expression>;" << endl;
                //print("<Assign> -> <ID> = <Expression>");
                currentNode->add(new Node(*record));
                finishNonTerminal(parent);
                return true;
            }
        }
    }
    cancelNonTerminal(parent);
    return false;
}
