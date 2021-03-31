#pragma once
#include <list>

struct ParseTreeNode {
    ParseTreeNode * parent;
    list<ParseTreeNode*> children;
};

class ParseTree {
    ParseTreeNode * root;
public:
    ParseTree() {

    }
};