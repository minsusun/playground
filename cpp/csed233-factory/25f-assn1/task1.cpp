#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

struct List {
    Node* head;
    List() : head(nullptr) {}

    void append(int val) {
        // TODO
    }

    void remove(int val) {
        // TODO
    }

    void print() {
        // TODO
    }
};

List unionList(List& l1, List& l2) {
    // TODO
    return List();
}

List intersectionList(List& l1, List& l2) {
    // TODO
    return List();
}

List differenceList(List& l1, List& l2) {
    // TODO
    return List();
}

int main() {
    int n, m, mode;
    cin >> n >> m >> mode;

    List l1, l2;
    for (int i = 0; i < n; i++) {
        int val; cin >> val;
        l1.append(val);
    }
    for (int j = 0; j < m; j++) {
        int val; cin >> val;
        l2.append(val);
    }

    List result;
    if (mode == 0) result = unionList(l1, l2);
    else if (mode == 1) result = intersectionList(l1, l2);
    else if (mode == 2) result = differenceList(l1, l2);

    result.print();

    return 0;
}
