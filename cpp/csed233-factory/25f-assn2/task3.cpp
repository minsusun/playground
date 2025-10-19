#include "ds.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

using namespace std;

// Build PQ from BinaryTree (values are node digits 0~9); heapified as MAX-HEAP
void PriorityQueue::buildFromBinaryTree(const BinaryTree& bt) {
    clear();
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////
    buildFromBinaryTreeInternal(bt.getRoot());
    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
}

bool PriorityQueue::insert(int value) {
    // if (...) return false; // TODO: fill in the condition
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////
    heap[size++].value = value;
    upBubble(size - 1);
    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
    return true;
}

bool PriorityQueue::deleteMax() {
    if (empty()) return false; // TODO: fill in the condition
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////
    heap[0] = heap[size - 1];
    size--;
    downBubble(0);
    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
    return true;
}

bool PriorityQueue::changeMax(int target) {
    if (empty()) return false; // TODO: fill in the condition
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////
    heap[0].value = target;
    downBubble(0);
    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
    return true;
}

pq_element PriorityQueue::getMax() {
    /////////////////////////////////////////////////////////
    //////////  TODO: Implement From Here      //////////////
    return heap[0];
    ///////////      End of Implementation      /////////////
    /////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////
//////////  You can implement any other functions ////////
void PriorityQueue::buildFromBinaryTreeInternal(Node* node) {
    if (node == NULL) return;
    buildFromBinaryTreeInternal(node -> left);
    buildFromBinaryTreeInternal(node -> right);
    insert(node -> value);
}

void PriorityQueue::upBubble(int index) {
    if (isRoot(index)) {
        return;
    }
    int parentIndex = parent(index);
    if (heap[index].value > heap[parentIndex].value) {
        int t = heap[index].value;
        heap[index].value = heap[parentIndex].value;
        heap[parentIndex].value = t;
    }
    upBubble(parentIndex);
}

void PriorityQueue::downBubble(int index) {
    if (isLeaf(index)) {
        return;
    }
    int leftIndex = left(index), rightIndex = right(index);
    int &value = heap[index].value;
    if (rightIndex >= size) {
        if (value < heap[leftIndex].value) {
            swap(value, heap[leftIndex].value);
            downBubble(leftIndex);
        }
    }
    else {
        int &value = heap[index].value, &lValue = heap[leftIndex].value, &rValue = heap[rightIndex].value;
        if (value >= lValue && value < rValue) {
            swap(value, rValue);
            downBubble(rightIndex);
        }
        else if (value >= rValue && value < lValue) {
            swap(value, lValue);
            downBubble(leftIndex);
        }
        else {
            int targetIndex = (rValue > lValue) ? rightIndex : leftIndex;
            swap(value, heap[targetIndex].value);
            downBubble(targetIndex);
        }
    }
}
///////////      End of Implementation      /////////////
/////////////////////////////////////////////////////////

/* This is given function. DO NOT MODIFY THIS FUNCTION */
string PriorityQueue::printPQ() {
    string answer;
    for (int i = 0; i < size; i++)
        answer += to_string(heap[i].value) + " ";
    return answer;
}

/* This is given function. DO NOT MODIFY THIS FUNCTION */
int findIndex(const char *str, int start, int end) {
    if (start > end) return -1;
    string s;
    for (int i = start; i <= end; i++) {
        if (str[i] == '(') s.push_back('(');
        else if (str[i] == ')') {
            if (!s.empty() && s.back() == '(') {
                s.pop_back();
                if (s.empty()) return i;
            }
        }
    }
    return -1;
}

/* This is given function. DO NOT MODIFY THIS FUNCTION */
Node* BinaryTree::_buildFromString(const char *data, int start, int end) {
    if (start > end) return NULL;
    while (start <= end && isspace((unsigned char)data[start])) start++;
    if (start > end || !isdigit((unsigned char)data[start])) return NULL;

    int i = start, val = 0;
    while (i <= end && isdigit((unsigned char)data[i])) {
        val = val * 10 + (data[i] - '0');
        i++;
    }
    Node *root = new Node(val);

    if (i <= end && data[i] == '(') {
        int mid = findIndex(data, i, end);
        root->left = _buildFromString(data, i + 1, mid - 1);
        int j = mid + 1;
        if (j <= end && data[j] == '(') {
            int rEnd = findIndex(data, j, end);
            root->right = _buildFromString(data, j + 1, rEnd - 1);
        }
    }
    return root;
}

/* This is given function. DO NOT MODIFY THIS FUNCTION */
void BinaryTree::buildFromString(const char *data) {
    Node *root = _buildFromString(data, 0, (int)strlen(data) - 1);
    _root = root;
}

/* This is given function. DO NOT MODIFY THIS FUNCTION */
int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: ./task3 <input_file>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin) {
        cerr << "Failed to open input file.\n";
        return 1;
    }

    ofstream fout("output3.txt");
    if (!fout) {
        cerr << "Failed to create output3.txt\n";
        return 1;
    }

    // Build a binary tree from a string
    string treeStr;
    getline(fin, treeStr); // full line contains the tree

    BinaryTree bt;
    bt.buildFromString(treeStr.c_str());

    // Tree -> Max-Heap
    PriorityQueue pq;
    pq.buildFromBinaryTree(bt);

    // Dynamic PQ (max-heap)
    int Q = 0;
    if (!(fin >> Q)) {
        fout << "Error\n";
        return 0;
    }

    string cmd;
    for (int i = 0; i < Q; i++) {
        if (!(fin >> cmd)) {
            fout << "Error\n";
            return 0;
        }

        if (cmd == "INSERT") {
            int v;
            if (!(fin >> v)) {
            fout << "Error\n";
            return 0;
            }
            pq.insert(v);
        } else if (cmd == "REMOVE_MAX") {
            pq.deleteMax();
        } else if (cmd == "GET_MAX") {
            if (pq.empty()) {
                fout << "EMPTY\n";
            } else {
                pq_element m = pq.getMax();
                fout << "MAX: " << m.value << "\n";
            }
        } else if (cmd == "CHANGE_MAX") {
            int t;
            if (!(fin >> t)) {
                fout << "Error\n";
                return 0;
            }
            if (!pq.changeMax(t)) {
                fout << "EMPTY\n";
            }
        } else if (cmd == "PRINT") {
            fout << "PQ: " << pq.printPQ() << "\n";
        } else {
            fout << "Error\n";
            return 0;
        }
        cout << pq.printPQ() << endl;
    }

    return 0;
}