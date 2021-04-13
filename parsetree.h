#pragma once
#include <list>

struct Node {
    Node * parent;
    string nonTerminal;
    Record token;
    list<Node*> children;

    Node(const string & rule) : children(), parent(nullptr), nonTerminal(rule) {

    }

    Node(Record & token) : token(token), children(), parent(nullptr) {

    }

    Node(Record * token) 
    : token(*token), 
      children(), 
      parent(nullptr) {

    }

    Node(const string & rule, Record & token) : nonTerminal(rule), token(token), children(), parent(nullptr) {

    }

    void add(Node * child) {
        child->parent = this;
        children.push_back(child);
    }
};

class ParseTree {
    Node * root;
public:
    ParseTree(): root(new Node("<Program>")) {

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

    void printRules() {
        return printRules(root);
    }

    void printTree() {
        return printTree(root);
    }

    void addToRoot(Node * child) {
        add(root, child);
    }

    void add(Node * parent, Node * child) {
        parent->add(child);
    }

    //Node * getLeftmostNode() {
    //    return getLeftmostNode(root);
    //}

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
            if (node->token.lexeme != ";")
                cout << " ";
            cout << node->token.lexeme;
            if (node->token.lexeme == ";")
                cout << endl;
        } else {
            if (showNonTerminal) {
                //if (!node->token.lexeme.empty()) {
                //    cout << node->token << endl;
                //}
                cout << "     " << node->nonTerminal << endl;
            }
            for (Node * child: node->children) {
                printNodes(child, showNonTerminal);
            }
        }
    }

    void printRules(Node * node, Record * currentRecord = nullptr) {
        if (node->children.empty()) {
            // this is a terminal node
            if (node->nonTerminal.empty()) {
            
                if (currentRecord != &node->token) {
                    cout << node->token << endl;
                    currentRecord = &node->token;
                }
            } else {
                cout << "     " << node->nonTerminal << endl;
            }

        } else {
            
            Node * leftNode = getLeftmostNode(node);
            if (&leftNode->token != currentRecord)
                cout << leftNode->token << endl;
            cout << "     " << node->nonTerminal << endl;
            currentRecord = &leftNode->token;
            
            for (auto it = node->children.begin(); it != node->children.end(); ++it) {
                printRules(*it, currentRecord);
            }
        }
    }

    Node * getLeftmostNode(Node * node) {
        if (node->children.empty()) {
            return node;
        }
        return getLeftmostNode(*node->children.begin());
    }

    void printTree(Node * node, int indent = 0) {
        if (node->children.empty()) {
            if (node->nonTerminal.empty()) {
                cout << string(indent * 2, ' ') << node->token.lexeme << endl;
            } else {
                cout << string(indent * 2, ' ') << node->nonTerminal << endl;
            }
        } else {
            cout << string(indent*2, ' ') << node->nonTerminal << endl;
            
            for (auto it = node->children.begin(); it != node->children.end(); ++it) {
                printTree(*it, indent + 1);
            }
        }
    }


};