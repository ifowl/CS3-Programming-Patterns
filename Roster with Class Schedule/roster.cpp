//Lab 4 - Roster with Class Schedule using list data structure
//Isaac Fowler
//
//roster.cpp

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <algorithm>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move;

// reading a list from a fileName
void readRoster(list<string>& roster, string fileName);

// same as above but takes vector as argument
void readRosterVector(vector<string>& roster, string fileName);

// printing a list out
void printRoster(const list<string>& roster);

int main(int argc, char* argv[]){

        if (argc <= 1){
                cout << "usage: " << argv[0]
                << " list of courses, dropouts last"
                << endl;
                exit(1);
        }

        // temporary vector of courses of students.
        vector <list<string>> tempStudentEntries;

        for(int i=1; i < argc-1; ++i){
                list<string> roster;
                readRoster(roster, argv[i]);
                tempStudentEntries.push_back(move(roster)); //Add all students found in each argument to the temp vector (including duplicates)
        }

        // master list of students
        list<string> allStudents;

        for(auto& lst : tempStudentEntries)
                allStudents.splice(allStudents.end(), lst);

        allStudents.sort(); //sorting master list lexicographically
        allStudents.unique();  //removing duplicates

        vector <list<string>> studentEntries;

        for (auto name : allStudents) {
                list<string> temp;
                temp.push_back(name);
                studentEntries.push_back(temp);
        }


        for(int i=1; i < argc-1; ++i){
                vector<string> rosterFromFile;
                readRosterVector(rosterFromFile, argv[i]);
                for (int j = 0; j < rosterFromFile.size(); ++j) { //iterates through every name in the file.
                        for (int k = 0; k < studentEntries.size(); ++k) { //iterates through every name students vector
                                if (rosterFromFile[j] == studentEntries[k].front()) {//if they are equal
                                        string className = argv[i];
                                        int pos = className.find('.');
                                        string className2 = className.substr(0, pos);
                                        studentEntries[k].push_back(className2); //add that course name to their student entry
                                }
                        }
                }
        }

        cout << endl << "All Students\nFirst name, last name:\tClasses" << endl;

        for (auto student : studentEntries) {
                printRoster(student);
        }

        // reading in dropouts
        list<string> dropouts;
        readRoster(dropouts, argv[argc-1]);
        //cout << "\n\n dropouts \n";
        //printRoster(dropouts);

        for (auto &dropName : dropouts) {
                for (int i = 0; i < studentEntries.size(); ++i) {
                        if (dropName == studentEntries[i].front()) {
                                studentEntries.erase(studentEntries.begin()+i);
                        }
                }
        }

        cout << endl << "Students still enrolled (Dropouts removed)\nFirst name, last name:\tClasses" << endl;

        for (auto student : studentEntries) {
                printRoster(student);
        }

}

// reading in a file of names into a list of strings
void readRoster(list<string>& roster, string fileName){
        ifstream course(fileName);
        string first, last;
        while(course >> first >> last)
                roster.push_back(first + ' ' + last);
        course.close();
}

// reading in a file of names into a vector of strings
void readRosterVector(vector<string>& roster, string fileName){
        ifstream course(fileName);
        string first, last;
        while(course >> first >> last)
                roster.push_back(first + ' ' + last);
        course.close();
}

// printing a list out
void printRoster(const list<string>& roster){
        for(const auto& str : roster)
                if (str == roster.front()) {
                        cout << str << ":";
                        if (roster.front().size() > 14)
                                cout << "\t";
                        else cout << "\t\t";
                }
                else
                        cout << str << " ";
        cout << endl;
}
