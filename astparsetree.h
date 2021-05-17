#pragma once
#include <list>
#include "parsetree.h"

class ASTParseTree : public ParseTree {

public:
    ASTParseTree(): ParseTree() {

    }

    void printNodes(ostream & stream, bool showNonTerminal = false) {
        stream << "-------Parse Tree Code-------------------" << endl;
        printNodes(stream, getRoot()->children.front(), showNonTerminal);
    }

    void printRules(ostream & stream) {
        stream << "-------Parse Tree Productions -----------" << endl;
        return printRules(stream, getRoot());
    }

    void printTree(ostream & stream) {
        stream << "-------Parse Tree -----------------------" << endl;
        return printTree(stream, getRoot(), 0);
    }

protected:

    void printNodes(ostream & stream, Node * node, bool showNonTerminal) override {
        if (node == nullptr)
            return;

        bool showParens = !node->token.lexeme.empty() && (node->token.lexeme == "+" || node->token.lexeme == "-");
        if (showParens) {
            stream << "(";
        }
        if (!node->children.empty()) {
            printNodes(stream, node->children.front(), showNonTerminal);
        }
        stream << node->token.lexeme << endl;
        if (!node->children.empty()) {
            printNodes(stream, node->children.back(), showNonTerminal);
        }
        if (showParens) {
            stream << ")";
        }
    }

    void printRules(ostream & stream, Node * node, Record * currentRecord = nullptr) override {
        if (node->children.empty()) {
            // this is a terminal node
            if (node->nonTerminal.empty()) {

                if (currentRecord != &node->token) {
                    stream << node->token << endl;
                    currentRecord = &node->token;
                }
            } else {
                stream << "     " << node->nonTerminal << endl;
            }

        } else {

            Node * leftNode = getLeftmostNode(node);
            if (&leftNode->token != currentRecord)
                stream << leftNode->token << endl;
            stream << "     " << node->nonTerminal << endl;
            currentRecord = &leftNode->token;

            for (auto & it : node->children) {
                printRules(stream, it, currentRecord);
            }
        }
    }

    void printTree(ostream & stream, Node * node, int indent) override {
        if (node->nonTerminal.empty() && !node->token.lexeme.empty()) {
            stream << string(indent * 2, ' ') << node->token.lexeme << endl;
        } else if (!node->nonTerminal.empty() && !node->token.lexeme.empty()) {
            stream << string(indent * 2, ' ') << node->token.lexeme << " : " << node->nonTerminal << endl;
        } else {
            stream << string(indent * 2, ' ') << node->nonTerminal << endl;
        }
        for (auto & it : node->children) {
            printTree(stream, it, indent + 1);
        }
    }
};