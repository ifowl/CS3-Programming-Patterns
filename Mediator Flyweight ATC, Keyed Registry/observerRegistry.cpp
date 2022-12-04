// Lab 13: Part Two - Modify code to make EventRegistery use strings as keys instead of pointers
// Isaac Fowler
//
// observerRegistry.cpp

#include <set>
#include <map>
#include <iostream>
#include <string>
#include <algorithm>

using std::cout; using std::cin; using std::endl;
using std::map; using std::set;
using std::string;

class Observer;
class Subject;

class EventRegistry{
public:
   static void registerObserver(string, Observer*);
   static void deregisterObserver(string, Observer*);
   static void handleMessage(string);
private:
   static map<string, set<Observer*>> observerMap_;
};

// initialize the static map
map<string, set<Observer*>> EventRegistry::observerMap_;


class Observer{
public:
   Observer(const string &name): name_(name) {}
   string getName() const {return name_;}
   void subscribe(string s) {EventRegistry::registerObserver(s, this);}
   void unsubscribe(string s){EventRegistry::deregisterObserver(s, this);}
   void handleMessage(string);
private:
   string name_;
};

class Subject{
public:
   Subject(const string &name): name_(name) {}
   string getName() const {return name_;}
   void generateMessage(){
      cout << name_ << " sent a message" << endl;
      EventRegistry::handleMessage(name_);}
private:
   string name_;
};

void EventRegistry::registerObserver(string s, Observer* o){
   observerMap_[s].insert(o);
   cout << o->getName() << " subscribed to " << s << endl;
}

void EventRegistry::deregisterObserver(string s, Observer* o){
   observerMap_[s].erase(o);
   cout << o->getName() << " unsubscribed from " << s << endl;
}

void EventRegistry::handleMessage(string s){
   for (auto e: observerMap_[s])
      e->handleMessage(s);
}

void Observer::handleMessage(string s) {
   cout << name_ << " received message from " << s << endl;
}


int main() {
   	Observer olaf("Observer Olaf"), olga("Observer Olga");
	string subjectName;
	cout << "Enter the name for your subject: ";
	cin >> subjectName;
	Subject sub(subjectName);

	olaf.subscribe(subjectName); olga.subscribe(subjectName);

	cout << endl;

	sub.generateMessage();
	olga.unsubscribe(subjectName);
	sub.generateMessage();
}
