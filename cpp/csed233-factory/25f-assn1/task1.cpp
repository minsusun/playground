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
        if (head == nullptr) {
            return;
        }
        Node* cursor = head;
        if (cursor -> data == val) {
            head = cursor -> next;
            delete cursor;
        }
        else {
            while (true) {
                if (cursor -> next == nullptr) {
                    return;
                }
                if (cursor -> next -> data == val) {
                    break;
                }
                cursor = cursor -> next;
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
            cursor = cursor -> next;
        }
        cout << endl;
    }
};

List unionList(List& l1, List& l2) {
    // TODO
    List result;
    Node* cursor_l1 = l1.head;
    Node* cursor_l2 = l2.head;
    auto cursor_move = [](List& l, Node* &cur) {
        int data = cur -> data;
        while (cur != nullptr && cur -> data == data) {
            cur = cur -> next;
        }
    };
    auto flush_left = [&](List& l, Node* &cur) {
        while (cur != nullptr) {
            result.append(cur -> data);
            cur = cur -> next;
        }
    };
    while (cursor_l1 != nullptr || cursor_l2 != nullptr) {
        if (cursor_l1 != nullptr && cursor_l2 != nullptr) {
            int data1 = cursor_l1 -> data, data2 = cursor_l2 -> data;
            if (data1 == data2) {
                result.append(data1);
                cursor_move(l1, cursor_l1);
                cursor_move(l2, cursor_l2);
            }
            else if (data1 > data2) {
                result.append(data2);
                cursor_move(l2, cursor_l2);
            }
            else {
                result.append(data1);
                cursor_move(l1, cursor_l1);
            }
        }
        else {
            if (cursor_l1 != nullptr) {
                flush_left(l1, cursor_l1);
            }
            else {
                flush_left(l2, cursor_l2);
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
    auto cursor_move = [](List& l, Node* &cur) {
        int data = cur -> data;
        while (cur != nullptr && cur -> data == data) {
            cur = cur -> next;
        }
    };
    while (cursor_l1 != nullptr && cursor_l2 != nullptr) {
        int data1 = cursor_l1 -> data, data2 = cursor_l2 -> data;
        if (data1 == data2) {
            result.append(data1);
            cursor_move(l1, cursor_l1);
            cursor_move(l2, cursor_l2);
        }
        else if (data1 > data2) {
            cursor_move(l2, cursor_l2);
        }
        else {
            cursor_move(l1, cursor_l1);
        }
    }
    return result;
}

List differenceList(List& l1, List& l2) {
    // TODO
    Node* cursor = l2.head;
    auto element_count = [](List& l, int val) {
        uint count = 0;
        Node* cur = l.head;
        while (cur != nullptr) {
            if (count > 0 && cur -> data != val) {
                break;
            }
            count += (cur -> data == val);
            cur = cur -> next;
        }
        return count;
    };
    while (cursor != nullptr) {
        int val = cursor -> data;
        for (int i = 0, c = element_count(l1, val); i < c; i++) {
            l1.remove(cursor -> data);
        }
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
