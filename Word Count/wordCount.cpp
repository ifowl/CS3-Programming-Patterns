//Lab 2 - Program that counts word occurrences in a file and sorts them.
//Isaac Fowler

//wordCount.cpp

#include "wordCount.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

using std::cout; using std::endl; using std::string;

//WordOccurrence Member Functions:
//--------------------------------

bool WordOccurrence::matchWord(const string &word) {
        if (word == word_)
                return true;
        else
                return false;
}

void WordOccurrence::increment() {
        num_++;
}

string WordOccurrence::getWord() const {
        return word_;
}

int WordOccurrence::getNum() const {
        return num_;
}

bool WordOccurrence::operator<(WordOccurrence rhs) const {
        if (num_ < rhs.getNum())
                return true;
        else return false;
}


//WordList Member Functions:
//--------------------------

//Default constructor
WordList::WordList(int size) {
        size_ = size;
        if (size > 0)
                wordArray_ = new WordOccurrence[size];
        else
                wordArray_ = nullptr;
}

//Copy constructor
WordList::WordList(const WordList &rhs) {
        size_ = rhs.size_;
        if (rhs.size_ > 0)
                wordArray_ = new WordOccurrence[rhs.size_];
        else
                wordArray_ = nullptr;

        for(int i = 0; i < size_; ++i)
                wordArray_[i] = rhs.wordArray_[i];
}


//Overloaded = operator for copy and swap idiom
WordList &WordList::operator=(WordList other) {

        int tempSize;
        tempSize = size_;
        size_ = other.size_;
        other.size_ = tempSize;

        WordOccurrence *tempArray;
        tempArray = wordArray_;
        wordArray_ = other.wordArray_;
        other.wordArray_ = tempArray;

        return *this;
}

//Destructor
WordList::~WordList() {
        delete [] wordArray_;
}


void WordList::wordSort() {
        std::sort(wordArray_, wordArray_ + size_);
}

void WordList::addWord(const string& word) {

        for (int i = 0; i < size_; ++i) {
                if (wordArray_[i].matchWord(word) == true) { //if a word matches a word already in our array
                        wordArray_[i].increment(); //increment it and return from addWord()
                        return;
                }
        }

        WordOccurrence *tmpWordArray = new WordOccurrence[size_ + 1]; //Create new temp wordArray_ of one size greater than current

        for (int i = 0; i < size_; ++i) {
                tmpWordArray[i] = wordArray_[i]; //copy over old word array to new word array
        }
        tmpWordArray[size_] = WordOccurrence(word, 1); //set newest added index of temp array to the word
        delete[] wordArray_; //deallocate old array
        wordArray_ = tmpWordArray; //Set it equal to the new array we just made
        ++size_; //increment the private variable size

}


void WordList::print() {
        for (int i = 0; i < size_; ++i) {
                cout << wordArray_[i].getWord();
                int wordSize = (wordArray_[i].getWord()).size();
                if (wordSize < 8)
                        cout << "\t\t\t";
                else if (wordSize >= 8 && wordSize < 16)
                        cout << "\t\t";
                else
                        cout << "\t";
                cout << wordArray_[i].getNum() << endl;
        }
}

bool equal(const WordList &lhs, const WordList &rhs) {
        if (lhs.size_ != rhs.size_)
                return false;
        int biggerSize = (lhs.size_ > rhs.size_ ? lhs.size_ : rhs.size_);
        for (int i = 0; i < biggerSize; ++i) {
                if (lhs.wordArray_[i].getWord() != rhs.wordArray_[i].getWord() || lhs.wordArray_[i].getNum() != rhs.wordArray_[i].getNum())
                        return false;
        }
        return true;
}



int main(int argc, char* argv[]) {

        WordList words;

        if (argc == 2) {

                string fileName = argv[1];
                string fileWord, alphaWord;

                std::ifstream file(fileName);

                while(file >> fileWord) { //While there is no failure taking input from the file
                        for (int i = 0; i < fileWord.size(); ++i) { //iterate through each word taken as input (character by character)
                                fileWord[i] = tolower(fileWord[i]); //Make all lowercase (to remove case sensitive word detection)
                                if (isalpha(fileWord[i]) != 0) //if the character is alpha numeric (not a number or punctuation)
                                                alphaWord.push_back(fileWord[i]); //add it to the alpha number string
                        }
                        words.addWord(alphaWord);
                        alphaWord = ""; //reset alphaWord to an empty string again.
                }
                words.wordSort();
                cout << "Searching the file \"" << fileName << "\" for word occurrences." << endl << endl;
                cout << "Word\t\t\tAmount" << endl;
                words.print();
        } else
                cout << "Error: Two arguments are required!" << endl;

}
