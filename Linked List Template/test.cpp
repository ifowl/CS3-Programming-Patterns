//Lab 3 - Templated Colleciton class that uses nodes to implement a singly linked list
//Isaac Fowler
//
//test.cpp

#include <iostream>
#include <string>
#include "node.hpp"
#include "listCollection.hpp"

using std::cout; using std::endl; using std::string;

int main() {
        Collection<int> intTest;
        cout << "creating a collection of ints" << endl;
        intTest.add(1);
        intTest.add(1);
        intTest.add(2);
        intTest.add(2);
        intTest.add(3);
        intTest.add(4);
        intTest.add(5);
        intTest.add(2);
        intTest.add(3);
        intTest.print();
        cout << "last node in intTest is: " << intTest.last() << endl;
        intTest.remove(2);
        cout << "- removed 2:" << endl;
        intTest.print();
        intTest.remove(1);
        cout << "- removed 1:" << endl;
        intTest.print();

        Collection<string> strTest;
        cout << "creating a collection of strings" << endl;
        strTest.add("Hello");
        strTest.add("world!");
        strTest.print();
        cout << "last node in strTest is: " << strTest.last() << endl;
}
