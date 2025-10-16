#pragma once
#include <iostream>
#include <string>
using namespace std;

#define MAX_SIZE 1000

// ===================== For Task 1 (BinaryTree base) =====================
struct Node {
    Node(int input_value) : value(input_value), left(NULL), right(NULL) {}
    int value;
    Node *left;
    Node *right;
};

class BinaryTree {
public:
    BinaryTree() { _root = NULL; }
    Node* getRoot() { return _root; }
    ~BinaryTree() { delete _root; _root = NULL; }

    // Build from inorder & preorder/postorder traversals
    void buildFromInPre(int* inorder, int* preorder, int n);
    void buildFromInPost(int* inorder, int* postorder, int n);

    // Calculate level with maximum width.
    void levelWithMaxWidth(int &level_out, int &width_out);
    
    // For task 3: Build from a parenthesized string like: 1(2()(4))(3()(5))
    // You do not need to implement the fucntion in task1.cpp.
    void buildFromString(const char *data);

    /////////////////////////////////////////////////////////
    //////  TODO: Add Public members if required ///////////

    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
private:
    Node *_root;

    /////////////////////////////////////////////////////////
    //////  TODO: Add Private members if required ///////////

    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////

    // For task 3: helpers
    // You do not need to implement the fucntion in task1.cpp.
    Node* _buildFromString(const char *data, int start, int end);
};

// ========================= For Task 2 (General Tree for LCA) =========================

class GeneralTree {
public:
    GeneralTree() : N(0), nodeCount(0) { }
    ~GeneralTree() { }
    void setN(int n) { N = n; }

    // Build tree structure and position maps from the DFS 0/1 sequence
    void buildFromSequence(const char* binseq, int len);

    // Decide which node to remove; returns (enterPos, exitPos) of that node
    void findRemoval(int X, int Y, int &outEnter, int &outExit) const;

    /////////////////////////////////////////////////////////
    //////  TODO: Add Public members if required ///////////

    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////

private:
    int N;                     // number of nodes
    int nodeCount;             // assigned node ids: 1..nodeCount
    /////////////////////////////////////////////////////////
    //////  TODO: Add Private members if required ///////////

    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
};

// ========================= For Task 3 (PriorityQueue) =========================

struct pq_element {
    int value;
};

class PriorityQueue {
public:
    PriorityQueue() : size(0) {}
    ~PriorityQueue() { clear(); }

    void buildFromBinaryTree(const BinaryTree& bt); // Heapify
    bool insert(int value); // Insert an element
    bool deleteMax(); // Remove the element with maximum priority
    bool changeMax(int target); // Change max element's priority to target, then fix heap
    pq_element getMax(); // Return the element with maximum priority
    string printPQ(); // Print payload values in array order

    // Utilities
    void clear() { size = 0; } // Make enpty
    bool empty() { return size == 0; } // Is empty?
    int getSize() const { return size; }
    const pq_element& at(int i) const { return heap[i]; }

    /////////////////////////////////////////////////////////
    //////  TODO: Add Public members if required ///////////

    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
    
private:
    pq_element heap[MAX_SIZE];
    int size;

    inline int parent(int i) const { return (i - 1) / 2; }
    inline int left(int i) const { return 2 * i + 1; }
    inline int right(int i) const { return 2 * i + 2; }

    
    /////////////////////////////////////////////////////////
    //////  TODO: Add Private members if required ///////////

    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
};

/////////////////////////////////////////////////////////
//////////  You can implement any other codes   /////////
//////////  Except for including STL libraries  ///////// 

///////////      End of Implementation      /////////////
/////////////////////////////////////////////////////////
