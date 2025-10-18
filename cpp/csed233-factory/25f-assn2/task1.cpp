#include "ds.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void BinaryTree::buildFromInPre(int* inorder, int* preorder, int n) {
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////
    _root = buildFromInPreInternal(inorder, preorder, n);
    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
}

void BinaryTree::buildFromInPost(int* inorder, int* postorder, int n) {
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////

    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
}


void BinaryTree::levelWithMaxWidth(int &level_out, int &width_out) {
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////

    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////
//////////  You can implement any other functions ////////
int BinaryTree::indexOfValueInInorder(int value, int* order, int n) {
    for (int i = 0; i < n; i++) {
        if (order[i] == value) {
            return i;
        }
    }
    return -1;
}

Node* BinaryTree::buildFromInPreInternal(int* inorder, int* preorder, int n) {
    if (n <= 0) {
        return NULL;
    }

    int rootValue = preorder[0];
    Node* root = new Node(rootValue);
    
    int rootIndex = indexOfValueInInorder(rootValue, inorder, n);
    

    int leftSubTreeCount = rootIndex;
    int rightSubTreeCount = n - 1 - leftSubTreeCount;

    root -> left = buildFromInPreInternal(inorder, preorder + 1, leftSubTreeCount);
    root -> right = buildFromInPreInternal(inorder + leftSubTreeCount + 1, preorder + 1 + leftSubTreeCount, rightSubTreeCount);

    return root;
}

Node* BinaryTree::buildFromInPostInternal(int* inorder, int* postorder, int n) {

}

///////////      End of Implementation      /////////////
/////////////////////////////////////////////////////////


/* This is given function. DO NOT MODIFY THIS FUNCTION */
static void parseLineToInts(const string &line, int arr[], int &cnt) {
    cnt = 0;
    int n = (int)line.size();
    int i = 0;
    while (i < n) {
        while (i < n && line[i] == ' ') ++i;
        if (i >= n) break;
        int sign = 1;
        if (line[i] == '-') { sign = -1; ++i; }
        if (i >= n || (line[i] < '0' || line[i] > '9')) {
            // 숫자가 아니면 스킵
            while (i < n && line[i] != ' ') ++i;
            continue;
        }
        int val = 0;
        while (i < n && line[i] >= '0' && line[i] <= '9') {
            val = val * 10 + (line[i] - '0');
            ++i;
        }
        arr[cnt++] = sign * val;
        while (i < n && line[i] == ' ') ++i;
    }
}

/* This is given function. DO NOT MODIFY THIS FUNCTION */
int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: ./task1 <input_file>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin) {
        cerr << "Failed to open input file.\n";
        return 1;
    }

    ofstream fout("output1.txt");
    if (!fout) {
        cerr << "Failed to create output1.txt\n";
        return 1;
    }

    string kind;
    string lineIn, lineOther;

    // Read inputs
    if (!getline(fin, kind)) return 0;
    if (!getline(fin, lineIn)) return 0;
    if (!getline(fin, lineOther)) return 0;

    int inorder[MAX_SIZE], other[MAX_SIZE];
    int nIn = 0, nOther = 0;
    parseLineToInts(lineIn, inorder, nIn);
    parseLineToInts(lineOther, other, nOther);

    if (nIn != nOther) {
        fout << "0 0";
        return 0;
    }

    // Build tree from traversals
    BinaryTree bt;
    if (kind == "PRE") {
        bt.buildFromInPre(inorder, other, nIn);
    } else { // kind == "POST"
        bt.buildFromInPost(inorder, other, nIn);
    }

    // Calculate level with maximum width
    int level, width;
    bt.levelWithMaxWidth(level, width);
    fout << level << " " << width;

    return 0;
}
