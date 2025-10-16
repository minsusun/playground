#include "ds.h"
#include <iostream>
#include <fstream>
#include <cstring>

void GeneralTree::buildFromSequence(const char* binseq, int len) {
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////

    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
}

void GeneralTree::findRemoval(int X, int Y, int &outEnter, int &outExit) const {
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////

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
