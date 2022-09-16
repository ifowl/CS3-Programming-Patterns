//Lab 3 - Templated Colleciton class that uses nodes to implement a singly linked list
//Isaac Fowler
//
//listCollection.hpp

#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include "list.hpp"

using std::cout; using std::endl;

//forward class declaration
template <typename T>
class Collection;

//forward function declaration
template <typename T>
bool equal(const Collection<T>&, const Collection<T>&);

//Collection class
template <typename T>
class Collection{
public:

        //default constructor
        Collection(): head_(nullptr) {}

        // member functions
        void add(const T item);
        void remove(const T item);
        T last();
        void print();

        //friend function
        friend bool equal<T>(const Collection<T>&, const Collection<T>&);

private:
        node<T> *head_;
};


//create a new node at the end of the list with given data
template <typename T>
void Collection<T>::add(const T item) {
        //Make new node
        node<T> *newNode = new node<T>;
        newNode->setData(item);
        //if the list is empty, assign the head as newNode.
        if (head_ == nullptr) {
                head_ = newNode;
        //if the list isn't empty, add it to the end.
        } else {
                //find end of the list
                node<T> *current = head_;
                while (current->getNext() != nullptr) {
                        current = current->getNext();
                }
                //make end of list point to newNode
                current->setNext(newNode);
        }
}

//iterate through list and remove every element that matches the argument given
template <typename T>
void Collection<T>::remove(const T item) {
        //if collection is empty do nothing
        if (head_ == nullptr)
                return;

        node<T> *iterator, *prev = head_;
        int i = 0; //counter to ensure prev stays behind iterator

        //iterate through the collection
        for (iterator = head_; iterator != nullptr; iterator=iterator->getNext()) {
                if (i > 1)
                        prev = prev->getNext(); //only move prev when iterator is ahead of it
                ++i;

                //if a match is found
                if (item == iterator->getData()) {
                        //if match is the head, then delete that node and create a temp node to store the iterator at. (avoids skipping nodes in collection)
                        if (iterator == head_) {
                                node<T> *temp = prev->getNext();
                                delete iterator;
                                node<T> *newHead;
                                newHead->setNext(temp);
                                iterator = newHead;
                                prev = temp;
                                head_ = temp;
                                i = 0;
                        //if match is elsewhere then delete it and make prev node point to the iterator's next node
                        } else {
                                node<T> *temp = iterator->getNext();
                                delete iterator;
                                iterator = prev;
                                prev->setNext(temp);
                                i = 1;
                        }
                }
        }
}

//iterate through collection and print out data of each node
template <typename T>
void Collection<T>::print() {
        node<T> *iterator = head_;

        for (iterator = head_; iterator != nullptr; iterator=iterator->getNext())
                cout << iterator->getData() << endl;
}

//Returns the data from the last node in a Collection
template <typename T>
T Collection<T>::last() {
        node<T> *current = head_;
        while (current->getNext() != nullptr) {
                current = current->getNext();
        }
        return current->getData();
}

//friend function definition
//Returns true or false if two collections have identical nodes
template <typename T>
bool equal(const Collection<T> &x, const Collection<T> &y) {
        node<T> *itx = x.head_;
        node<T> *ity = y.head_;
        //Loop until the end is reached and as long as their data is equal
        while ((itx->getNext() != nullptr) && (itx->getData() == ity->getData())) {
                        itx = itx->getNext();
                        ity = ity->getNext();
        }
        //If the final data is equal after reaching then end, then return true
        return (itx->getData() == ity->getData());
}


#endif
