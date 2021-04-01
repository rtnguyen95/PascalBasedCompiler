#pragma once
#include <list>

struct Node {
    Node * parent;
    string nonTerminal;
    Record token;
    list<Node*> children;

    Node(const string & name) : children(), parent(nullptr), nonTerminal(name) {

    }

    Node(Record & token) : token(token), children(), parent(nullptr) {

    }

    void add(Node * child) {
        child->parent = this;
        children.push_back(child);
    }
};

class ParseTree {
    Node * root;
public:
    ParseTree(): root(new Node("root")) {

    }

    ~ParseTree() {
        deleteAllNodes();
    }

    Node * getRoot() {
        return root;
    }

    void printNodes(bool showNonTerminal = false) {
        printNodes(root, showNonTerminal);
    }

    void addToRoot(Node * child) {
        add(root, child);
    }

    void add(Node * parent, Node * child) {
        parent->add(child);
    }

protected:
    void deleteAllNodes() {
        deleteAllNodes(root);
    }

    void deleteAllNodes(Node * node) {
        for (Node * child: node->children) {
            deleteAllNodes(child);
        }
        delete node;
    }

    void printNodes(Node * node, bool showNonTerminal) {
        if (node->children.empty()) {
            cout << " " << node->token.lexeme;
            if (node->token.lexeme == ";")
                cout << endl;
        } else {
            if (showNonTerminal) {
                cout << "<" << node->nonTerminal << ">" << endl;
            }
            for (Node * child: node->children) {
                printNodes(child, showNonTerminal);
            }
        }
    }
};