#ifndef _PROB1_H
#define _PROB1_H

#include <limits>

struct Node{
    int data;
    Node* next = nullptr;
};

struct OrderedList{
    int m_size = 0;
    Node* head = nullptr;
};

void add(OrderedList* ordered, int v);
void add(OrderedList* ordered, const int* arr, int size);

void remove(OrderedList* ordered, int index);

int size(OrderedList* ordered);

bool contains(OrderedList* ordered, int v);

int getValue(OrderedList* ordered, int idx);

#endif