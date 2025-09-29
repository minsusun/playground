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
        Node* n = new Node(val);
        Node* cursor = head;
        if (head == nullptr) {
            head = n;
        }
        else {
            while (cursor -> next != nullptr) {
                cursor = cursor -> next;
            }
            cursor -> next = n;
        }
    }

    void remove(int val) {
        // TODO
        Node* cursor = head;
        if (cursor -> data == val) {
            head = cursor -> next;
            delete cursor;
        }
        else {
            while (cursor -> next -> data != val) {
                cursor = cursor -> next;
                if (cursor -> next == nullptr) {
                    return;
                }
            }
            Node* target = cursor -> next;
            cursor -> next = target -> next;
            delete target;
        }
    }

    void print() {
        // TODO
        Node *cursor = head;
        while (cursor != nullptr) {
            cout << cursor -> data;
            if (cursor -> next != nullptr) {
                cout << " ";
            }
            else {
                cout << "\n";
            }
            cursor = cursor -> next;
        }
    }
};

List unionList(List& l1, List& l2) {
    // TODO
    List result;
    Node* cursor_l1 = l1.head;
    Node* cursor_l2 = l2.head;
    while (cursor_l1 != nullptr || cursor_l2 != nullptr) {
        if (cursor_l1 != nullptr && cursor_l2 != nullptr && cursor_l1 -> data == cursor_l2 -> data) {
            int data = cursor_l1 -> data;
            result.append(cursor_l1 -> data);
        }
        else {
            if (cursor_l1 != nullptr) {
                result.append(cursor_l1 -> data);
            }
            if (cursor_l2 != nullptr){
                result.append(cursor_l2 -> data);
            }
        }
        if (cursor_l1 != nullptr) {
            int data = cursor_l1 -> data;
            while (cursor_l1 != nullptr && cursor_l1 -> data == data) {
                cursor_l1 = cursor_l1 ->next;
            }
        }
        if (cursor_l2 != nullptr) {
            int data = cursor_l2 -> data;
            while (cursor_l2 != nullptr && cursor_l2 -> data == data) {
                cursor_l2 = cursor_l2 ->next;
            }
        }
    }
    return result;
}

List intersectionList(List& l1, List& l2) {
    // TODO
    List result;
    Node* cursor_l1 = l1.head;
    Node* cursor_l2 = l2.head;
    while (cursor_l1 != nullptr && cursor_l2 != nullptr) {
        if (cursor_l1 -> data == cursor_l2 -> data) {
            result.append(cursor_l1 -> data);
            {
                int data = cursor_l1 -> data;
                while (cursor_l1 != nullptr && cursor_l1 -> data == data) {
                    cursor_l1 = cursor_l1 ->next;
                }
            }
            {
                int data = cursor_l2 -> data;
                while (cursor_l2 != nullptr && cursor_l2 -> data == data) {
                    cursor_l2 = cursor_l2 ->next;
                }
            }
        }
        else if (cursor_l1 -> data > cursor_l2 -> data) {
            int data = cursor_l2 -> data;
            while (cursor_l2 != nullptr && cursor_l2 -> data == data) {
                cursor_l2 = cursor_l2 ->next;
            }
        }
        else {
            int data = cursor_l1 -> data;
            while (cursor_l1 != nullptr && cursor_l1 -> data == data) {
                cursor_l1 = cursor_l1 ->next;
            }
        }
    }
    return result;
}

List differenceList(List& l1, List& l2) {
    // TODO
    Node* cursor = l2.head;
    while (cursor != nullptr) {
        l1.remove(cursor -> data);
        cursor = cursor -> next;
    }
    return l1;
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
