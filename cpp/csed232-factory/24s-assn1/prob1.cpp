#include "prob1.h"

#include <iostream>

void add(OrderedList* ordered, int v) {

    if (ordered->head == nullptr) {
        Node *node = new Node;
        node->data = v;
        ordered->head = node;
    }
    else if (ordered->head->data > v) {
        // HEAD 이전에 새로 데이터가 추가되는 경우
        Node *node = new Node;
        node->data = v;
        node->next = ordered->head;
        ordered->head = node;
    }
    else {
        // 그 이외에 중간에 추가되거나 맨 끝에 추가되는 경우
        Node *current = ordered->head;
        while (current->next != nullptr && current->next->data < v) {
            current = current->next;
        }
        Node *node = new Node;
        node->data = v;
        node->next = current->next;
        current->next = node;
    }
    ordered->m_size++;
}

void add(OrderedList* ordered, const int* arr, int size) {
    for(int i = 0; i < size; i++) add(ordered, arr[i]);
}

void remove(OrderedList* ordered, int index) {
    if (index >= ordered->m_size || index < 0)
        return;

    if (index == 0) {
        Node *current = ordered->head;
        ordered->head = current->next;
        delete(current);
    }
    else {
        Node *current = ordered->head;
        for(int i = 0; i < index - 1; i++)
            current = current->next;
        Node *t = current->next;
        current->next = current->next->next;
        delete(t);
    }
    ordered->m_size--;
}

int size(OrderedList* ordered) {
    return ordered->m_size;
}

bool contains(OrderedList* ordered, int v) {
    Node* current = ordered->head;
    for (int i = 0; i < ordered->m_size; i++) {
        if (v == current->data) return true;
        current = current->next;
    }
    return false;
}

int getValue(OrderedList* ordered, int idx) {
    if (idx >= ordered->m_size || idx < 0)
        return std::numeric_limits<int>::min();

    Node *current = ordered->head;
    for(int i = 0; i < idx; i++) current = current->next;
    return current->data;
}