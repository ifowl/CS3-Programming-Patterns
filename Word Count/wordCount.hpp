//Lab 2 - Program that counts word occurrences in a file and sorts them.
//Isaac Fowler
//
//wordCount.hpp

#ifndef wordCount_HPP
#define wordCount_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

using std::string;

class WordOccurrence {
public:
        WordOccurrence(const string& word="", int num=0) : word_(word), num_(num){} //initializer list
        bool matchWord(const string &); // returns true if word matches stored
        void increment(); // increments number of occurrences
        string getWord() const;
        int getNum() const;
        bool operator<(WordOccurrence) const;

private:
        string word_;
        int num_;
};

class WordList{
public:
        // add copy constructor, destructor, overloaded assignment
        WordList(int size = 0); //default constructor
        WordList(const WordList &rhs); //copy constructor
        ~WordList(); //destructor

        WordList &operator=(WordList); //operator overloading

        // implement comparison as friend
        friend bool equal(const WordList&, const WordList&);

        void wordSort();

        void addWord(const string &);
        void print();
private:
        WordOccurrence *wordArray_; // a dynamically allocated array of WordOccurrences
        int size_;
};


#endif
