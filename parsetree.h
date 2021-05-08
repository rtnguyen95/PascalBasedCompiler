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

    Node * add(Node * child) {
        child->parent = this;
        children.push_back(child);
        return child;
    }

    Node * addFirst(Node * child) {
        child->parent = this;
        children.push_front(child);
        return child;
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

    virtual void printNodes(ostream & stream, bool showNonTerminal = false) {
        stream << "-------Parse Tree Code-------------------" << endl;
        printNodes(stream, root, showNonTerminal);
    }

    virtual void printRules(ostream & stream) {
        stream << "-------Parse Tree Productions -----------" << endl;
        return printRules(stream, root);
    }

    virtual void printTree(ostream & stream) {
        stream << "-------Parse Tree -----------------------" << endl;
        return printTree(stream, root);
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

    virtual void printNodes(ostream & stream, Node * node, bool showNonTerminal) {
        if (node->children.empty()) {
            if (node->token.lexeme != ";")
                stream << " ";
            stream << node->token.lexeme;
            if (node->token.lexeme == ";")
                stream << endl;
        } else {
            if (showNonTerminal) {
                stream << "     " << node->nonTerminal << endl;
            }
            for (Node * child: node->children) {
                printNodes(stream, child, showNonTerminal);
            }
        }
    }

    virtual void printRules(ostream & stream, Node * node, Record * currentRecord = nullptr) {
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
            
            for (auto it = node->children.begin(); it != node->children.end(); ++it) {
                printRules(stream, *it, currentRecord);
            }
        }
    }

    Node * getLeftmostNode(Node * node) {
        if (node->children.empty()) {
            return node;
        }
        return getLeftmostNode(*node->children.begin());
    }

    virtual void printTree(ostream & stream, Node * node, int indent = 0) {
        if (node->children.empty()) {
            if (node->nonTerminal.empty()) {
                stream << string(indent * 2, ' ') << node->token.lexeme << endl;
            } else {
                stream << string(indent * 2, ' ') << node->nonTerminal << endl;
            }
        } else {
            stream << string(indent*2, ' ') << node->nonTerminal << endl;
            
            for (auto it = node->children.begin(); it != node->children.end(); ++it) {
                printTree(stream, *it, indent + 1);
            }
        }
    }
};