//Lab 5 - Roster with Class Schedule using Map of student classes and list of strings
//Isaac Fowler
//
//rosterMap.cpp

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <cstdlib>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move;
using std::map;


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
};

//adding students into vector
void readRosterStudent(vector<Student>& roster, string fileName);

//adding students into map keyed by Student
void readRosterMap(map<Student,list<string>>& roster, string fileName);

//printing the map
void printRoster(const map<Student,list<string>>);

int main(int argc, char* argv[]){

        if (argc <= 1){
                cout << "usage: " << argv[0]
                << " list of courses, dropouts last"
                << endl;
                exit(1);
        }
	
	//Map for storing Students and their classes
	map<Student, list<string>> studentMap;

	//Read all names into the map from the files given as command line arguments
        for(int i=1; i < argc-1; ++i){
		readRosterMap(studentMap, argv[i]);
        }
		
	//Add classes to each Student entry
        for(int i=1; i < argc-1; ++i){
                vector<Student> rosterFromFile;
                readRosterStudent(rosterFromFile, argv[i]); //Put every name into a vector of Student classes
                for (int j = 0; j < rosterFromFile.size(); ++j) { //iterates through every name in vector.
			string className = argv[i];
			className = className.substr(0, className.find('.'));
			studentMap[rosterFromFile[j]].push_back(className); //Add their class name to the list of classes
                }
        }
	
	cout << endl << "All Students\nFirst name, last name:\tClasses" << endl;
	printRoster(studentMap);

        // reading in dropouts
        vector<Student> dropouts;
        readRosterStudent(dropouts, argv[argc-1]);
	
	//Erase dropouts from the student map
        for (auto dropName : dropouts) {
		studentMap.erase(dropName);
        }

	cout << endl << "Students still enrolled (Dropouts removed)\nFirst name, last name:\tClasses" << endl;
	printRoster(studentMap);	
}

void readRosterMap(map<Student,list<string>>& roster, string fileName) {
	ifstream course(fileName);
	string first, last;
	list<string> emptyList;
	while(course >> first >> last) {
		Student temp(first, last);
		roster.insert(make_pair(temp, emptyList));
	}
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

void printRoster(const map<Student,list<string>> studentMap){
	for (auto stu : studentMap) {
        	cout << stu.first.getName() << ":";
		if (stu.first.getName().size() > 14)
			cout << "\t";
		else cout << "\t\t";
        	for (auto course : stu.second)
        		cout << course << " ";
                cout << endl;
        }
}
