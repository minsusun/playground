#include "ds.h"
#include <iostream>
#include <fstream>
#include <cstring>

void GeneralTree::buildFromSequence(const char* binseq, int len) {
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////
    int st[MAX_SIZE + 5];
    int sp = -1;
    for (int i = 0; i < len; i++) {
        if (binseq[i] == '0') {
            st[++sp] = i + 1;
        }
        else {
            indexMap[i + 1] = st[sp];
            indexMap[st[sp--]] = i + 1;
        }
    }
    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
}

void GeneralTree::findRemoval(int X, int Y, int &outEnter, int &outExit) const {
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////
    // SAME POTATO, REMOVE ONLY ONE
    if (indexMap[X] == Y) {
        outEnter = max(X, Y);
        outExit = max(X, Y);
        return;
    }

    int XEnter = min(X, indexMap[X]), XExit = max(X, indexMap[X]);
    int YEnter = min(Y, indexMap[Y]), YExit = max(Y, indexMap[Y]);

    // Y beloongs to subtree of X, REMOVE X
    if (XEnter < YEnter && XExit > YExit) {
        outEnter = XEnter;
        outExit = XExit;
    }
    // X beloongs to subtree of Y, REMOVE Y
    else if (XEnter > YEnter && XExit < YExit) {
        outEnter = YEnter;
        outExit = YExit;
    }
    // FIND LCA
    else {
        int left = min(XEnter, YEnter);
        int right = max(XExit, YExit);
        int cursor = left - 1;
        while (true) {
            if (cursor < left && indexMap[cursor] > right) break;
            cursor --;
        }
        outEnter = cursor;
        outExit = indexMap[cursor];
    }
    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////
//////////  You can implement any other functions ////////

///////////      End of Implementation      /////////////
/////////////////////////////////////////////////////////

/* This is given function. DO NOT MODIFY THIS FUNCTION */
int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: ./task2 <input_file>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin) {
        cerr << "Failed to open input file.\n";
        return 1;
    }

    ofstream fout("output2.txt");
    if (!fout) {
        cerr << "Failed to create output2.txt\n";
        return 1;
    }

    int N;
    char seq[2 * MAX_SIZE + 5];
    int X, Y;

    // Read inputs
    if (!(fin >> N)) return 0;
    if (!(fin >> seq)) return 0;
    if (!(fin >> X >> Y)) return 0;

    // Build tree from sequence
    GeneralTree gt;
    gt.setN(N);
    gt.buildFromSequence(seq, 2 * N);

    // Compute node to remove
    int iPos = 0, jPos = 0;
    gt.findRemoval(X, Y, iPos, jPos);

    fout << iPos << ' ' << jPos;

    return 0;
}
