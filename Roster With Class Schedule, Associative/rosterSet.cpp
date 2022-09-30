//Lab 5 - Roster of students enrolled using a set container
//Isaac Fowler
//
//rosterSet.cpp

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <set>
#include <cstdlib>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move;
using std::set;


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

//adding students into set
void readRosterSet(set<Student>& roster, string fileName);

//removing students from set
void deleteRosterSet(set<Student>& roster, string fileName);

// printing a set out
void printRoster(const set<Student>);

int main(int argc, char* argv[]){

        if (argc <= 1){
                cout << "usage: " << argv[0]
                << " list of courses, dropouts last"
                << endl;
                exit(1);
        }
	
	//Map for storing Students and their classes
	set<Student> students;
	
	
	//Read all names into the set from the files given as command line arguments
        for(int i=1; i < argc-1; ++i){
		readRosterSet(students, argv[i]);
        }

	//erase students by key from set inside dropouts file (given by last argument)
	deleteRosterSet(students, argv[argc-1]);

	cout << endl << "Currently Enrolled Students" << endl;
	printRoster(students);
}

void readRosterSet(set<Student>& roster, string fileName) {
	ifstream course(fileName);
	string first, last;
	list<string> emptyList;
	while(course >> first >> last) {
		Student temp(first, last);
		roster.insert(temp);
	}
	course.close();
}

void deleteRosterSet(set<Student>& roster, string fileName){
        ifstream course(fileName);
        string first, last;
        while(course >> first >> last) {
		Student temp(first, last);
                roster.erase(temp);
	}
        course.close();
}

void printRoster(const set<Student> roster){
	for (auto stu : roster) {
        	cout << stu.getName() << endl;
        }
}
