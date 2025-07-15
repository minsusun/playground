#include "prob1.h"
#include <iostream>
void simpleTest1(OrderedList *o)
{
    add(o, 5);
    add(o, 4);
    add(o, 3);
    add(o, 80);
    add(o, 50);
    for (int i = 0; i < o->m_size; ++i)
    {
        std::cout << getValue(o, i) << ", ";
    }
    std::cout << std::endl;
}
void simpleTest2(OrderedList *o)
{
    int vals[] = {10, 20, 35, 35, 10};
    add(o, vals, sizeof(vals) / sizeof(int));
    for (int i = 0; i < o->m_size; ++i)
    {
        std::cout << getValue(o, i) << ", ";
    }
    std::cout << std::endl;
}
void simpleTest3(OrderedList *o)
{
    remove(o, 3);
    for (int i = 0; i < o->m_size; ++i)
    {
        std::cout << getValue(o, i) << ", ";
    }
    std::cout << std::endl;
}
void simpleTest4(OrderedList *o)
{
    std::cout << std::boolalpha << contains(o, 20) << std::endl;
    std::cout << std::boolalpha << contains(o, 40) << std::endl;
}
void simpleTest5(OrderedList *o)
{
    std::cout << size(o) << std::endl;
}

int main()
{
    OrderedList orderedList;
    std::cout << "<<Simple Test 1>>" << std::endl;
    simpleTest1(&orderedList);
    std::cout << "<<Simple Test 2>>" << std::endl;
    simpleTest2(&orderedList);
    std::cout << "<<Simple Test 3>>" << std::endl;
    simpleTest3(&orderedList);
    std::cout << "<<Simple Test 4>>" << std::endl;
    simpleTest4(&orderedList);
    std::cout << "<<Simple Test 5>>" << std::endl;
    simpleTest5(&orderedList);
    return 0;
}