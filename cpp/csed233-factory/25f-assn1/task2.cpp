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
        Node* n = new Node(expr, prec);
        n -> next = top;
        top = n;
    }

    Node* pop() {
        // TODO
        Node* result = top;
        if (top != nullptr) {
            top = top -> next;
        }
        return result;
    }

    bool isEmpty() {
        // TODO
        return top == nullptr;
    }
};

int precedence(char op) {
    // TODO
    if (op == '+' || op == '-') {
        return 1;
    }
    return 2;
}

int main() {
    string postfix;
    cin >> postfix;

    Stack s;

    for (char c : postfix) {
        if (c != '+' && c != '-' &&  c != '*' && c != '/') {
            s.push(string(1, c), 3);
        } else {
            Node* right = s.pop();
            Node* left = s.pop();
            int currPrec = precedence(c);

            string leftExpr = left->expr;
            if (left->prec < currPrec) {
                leftExpr = "(" + leftExpr + ")";
            }

            string rightExpr = right->expr;
            if (right->prec < currPrec || (right->prec == currPrec && (c == '-' || c == '/'))) {
                rightExpr = "(" + rightExpr + ")";
            }

            string expr = leftExpr + c + rightExpr;
            s.push(expr, currPrec);

            delete left;
            delete right;
        }
    }

    Node* result = s.pop();
    cout << result->expr << endl;
    delete result;

    return 0;
}
