//Lab 4 - Roster with Class Schedule using Student class
//Isaac Fowler
//
//rosterObject.cpp


#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move;


class Student{
public:
        Student(string firstName, string lastName):
                firstName_(firstName), lastName_(lastName) {}

        // move constructor, not really needed, generated automatically
        Student(Student && org):
                firstName_(move(org.firstName_)),
                lastName_(move(org.lastName_))
                {}

        // move assignment
        Student& operator=(Student&& other) {
                firstName_ = other.firstName_;
                lastName_ = other.lastName_;
                return (*this);
        }


        // force generation of default copy constructor
        Student(const Student & org) = default;

        string getName() const {return firstName_ + ' ' + lastName_;}

        void addClass(string course) { classes_.push_back(course); }

        void print() const;

        // needed for unique() and for remove()
        friend bool operator== (Student left, Student right){
        return left.lastName_ == right.lastName_ &&
                left.firstName_ == right.firstName_;
        }

        // needed for sort()
        friend bool operator< (Student left, Student right){
                return left.lastName_ < right.lastName_ ||
                (left.lastName_ == right.lastName_ &&
                left.firstName_ < right.firstName_);
        }
private:
        string firstName_;
        string lastName_;
        list<string> classes_;
};

void Student::print() const {
        string fullName = firstName_ + " " + lastName_;
        cout << fullName + ":";
        if (fullName.size() > 14)
                cout << "\t";
        else cout << "\t\t";
        for (auto course : classes_)
                cout << course << " ";

        cout << endl;
}


// reading a list from a fileName
void readRoster(list<string>& roster, string fileName);

// same as above but takes vector as argument
void readRosterVector(vector<string>& roster, string fileName);

//adding students into vector
void readRosterStudent(vector<Student>& roster, string fileName);

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
        vector<list<string>> tempStudentEntries;

        for(int i=1; i < argc-1; ++i){
                list<string> roster;
                readRoster(roster, argv[i]);
                tempStudentEntries.push_back(move(roster)); //Add all students found in each argument to the temp vector (including duplicates)
        }


        // master list of students
        list<string> allStudents;

        // Actual vector of student objects
        vector<Student> studentEntries;

        //Splice into temp entries
        for(auto& lst : tempStudentEntries)
                allStudents.splice(allStudents.end(), lst);

        allStudents.sort(); //Sort lexicographically
        allStudents.unique(); //elimate duplicates

        //Copy from list of strings to vector of Students
        for (auto student : allStudents) {
                int pos = student.find(' ');
                string firstName = student.substr(0, pos);
                string lastName = student.substr(pos + 1);
                Student temp(firstName, lastName);
                studentEntries.push_back(temp);
        }

        //print vector
        //cout << "Printing everyone in the vector" << endl;
        //for (auto student : studentEntries) {
        //      cout << student.getName() << endl;
        //}

        //Add classes to each Student entry
        for(int i=1; i < argc-1; ++i){
                vector<string> rosterFromFile;
                readRosterVector(rosterFromFile, argv[i]);
                for (int j = 0; j < rosterFromFile.size(); ++j) { //iterates through every name in the file.
                        for (int k = 0; k < studentEntries.size(); ++k) { //iterates through every name in students vector
                                if (rosterFromFile[j] == studentEntries[k].getName()) {//if they are equal
                                        string className = argv[i];
                                        int pos = className.find('.');
                                        string className2 = className.substr(0, pos);
                                        studentEntries[k].addClass(className2); //add that course name to their student entry
                                }
                        }
                }
        }



        cout << endl << "All Students\nFirst name, last name:\tClasses" << endl;

        for (auto student : studentEntries) {
                student.print();
        }


        // reading in dropouts
        vector<Student> dropouts;
        readRosterStudent(dropouts, argv[argc-1]);

        //Erase dropouts from the Student vector
        for (auto &dropName : dropouts) {
                for (int i = 0; i < studentEntries.size(); ++i) {
                        if (dropName.getName() == studentEntries[i].getName()) {
                                studentEntries.erase(studentEntries.begin() + i);
                        }
                }
        }


        cout << endl << "Students still enrolled (Dropouts removed)\nFirst name, last name:\tClasses" << endl;

        for (auto student : studentEntries) {
                student.print();
        }

}

void readRoster(list<string>& roster, string fileName){
        ifstream course(fileName);
        string first, last;
        while(course >> first >> last)
                roster.push_back(first + ' ' + last);
        course.close();
}

void readRosterStudent(vector<Student>& roster, string fileName){
        ifstream course(fileName);
        string first, last;
        while(course >> first >> last) {
                Student temp(first, last);
                roster.push_back(temp);
        }
        course.close();
}

void readRosterVector(vector<string>& roster, string fileName){
        ifstream course(fileName);
        string first, last;
        while(course >> first >> last)
                roster.push_back(first + ' ' + last);
        course.close();
}
