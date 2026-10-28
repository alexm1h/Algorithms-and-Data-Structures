#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

class Node {
    char d;
    Node *lft;
    Node *rgt;
public:
    Node(char tag = ' ') : d(tag), lft(nullptr), rgt(nullptr) {}
    ~Node() {
        delete lft;
        delete rgt;
    }
    friend class Tree;
};

class Tree {
    Node *root;
    char current_tag;
    const char max_tag;
    const int max_depth;
    const int max_nodes;
    int node_count;
    char **SCREEN;
    const int screen_rows;
    const int screen_cols;

    void clearScreen();
    Node* createNode(int depth);
    void drawNode(Node *v, int row, int col, int offset);
    int countLeftLeaves(Node *v);
    int calculateHeight(Node *v);

public:
    Tree(char start_tag, char end_tag, int max_depth, int max_nodes, int screen_rows = 12, int screen_cols = 80);
    ~Tree();
    void generateTree();
    bool isEmpty() { return root == nullptr; }
    int countLeftLeavesDFS();
    void displayTree();
    int getNodeCount() { return node_count; }
    int getTreeHeight();
};

#endif
