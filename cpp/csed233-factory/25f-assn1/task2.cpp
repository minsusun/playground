#include <iostream>
#include <string>
using namespace std;

struct Node {
    string expr;
    int prec;
    Node* next;
    Node(string e, int p) : expr(e), prec(p), next(nullptr) {}
};

struct Stack {
    Node* top;
    Stack() : top(nullptr) {}

    void push(string expr, int prec) {
        // TODO
    }

    Node* pop() {
        // TODO
        return nullptr;
    }

    bool isEmpty() {
        // TODO
        return true;
    }
};

int precedence(char op) {
    // TODO
    return 0;
}

int main() {
    string postfix;
    cin >> postfix;

    Stack s;

    // TODO: implement postfix to minimal infix conversion

    return 0;
}
