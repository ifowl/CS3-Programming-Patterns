// Lab 10: Part 2 - state design pattern
// Isaac Fowler
//
// StateTransition.cpp

#include <iostream>
#include <string>
using std::cin; using std::cout; using std::endl; using std::string;

class State;

// context
class Process{
public:
   Process();

   // behaviors
   void disbatch();
   void block();
   void unblock();
   void suspend();
   void exit();
   string report();

   string getID() { return std::to_string(ID_); }
   // part of implementation of state pattern
   void changeState(State* state){state_=state;}
private:
   State* state_;
   static int nextID_;
   int ID_;
};

// absract state
class State{
public:
   // provides default implementation
   virtual void disbatch(Process*){} 
   virtual void suspend(Process*){}
   virtual void block(Process*){}
   virtual void unblock(Process*){}
   virtual void exit(Process*){}
   virtual string report() =0;
   void changeState(Process* p, State* s){
      p->changeState(s);
   }
   string getID(Process* p){
	   return p->getID();
   }
};

class Ready: public State{
public:
   static State* instance(){
      static State* onlyInstance = new Ready;
      return onlyInstance;
   }
   void disbatch(Process*) override;  
   string report() override {
	   return "ready";
   }
private:
   // here and elsewhere should be stated private constructor/assignment
   // to correctly implement singleton, skipped to simplify code
};


class Running: public State{
public:
   static State* instance(){
      static State* onlyInstance = new Running;
      return onlyInstance;
   }
   void block(Process*) override;
   void suspend(Process*) override;
   //void exit(Process*) override;
   string report() override {return "running";}
};


class Blocked: public State{
public:
   static State* instance(){
      static State* onlyInstance = new Blocked;
      return onlyInstance;
   }
   void unblock(Process*) override;
   string report() override {return "blocked";}
};


// state transitions member functions

void Ready::disbatch(Process *p) {
    changeState(p, Running::instance());
} 
void Running::block(Process *p) {
   changeState(p, Blocked::instance());
}
void Running::suspend(Process *p) {
   changeState(p, Ready::instance());
}
//void Running::exit(Process *p) {
//	cout << "Process " << p->getID() << " destroyed";
//}
void Blocked::unblock(Process *p) {
   changeState(p, Ready::instance());
}

// Process member functions
Process::Process(){
	state_=Ready::instance();
	ID_ = nextID_++;
}
int Process::nextID_ = 1;

// handles/behaviors
void Process::disbatch(){state_->disbatch(this);}
void Process::suspend(){state_->suspend(this);}
void Process::block(){state_->block(this);}
void Process::unblock(){state_->unblock(this);}
void Process::exit(){cout << "Process " << getID() << " destroyed" << endl;}
string Process::report(){return state_->report();}

int main(){
   Process p1;
   cout << "Process created" << endl;
   cout << "ID: " << p1.getID() << endl;
   bool exit = false;
   while (!exit){
	   char action;
	   cout << p1.report() << endl;
	   if (p1.report() == "ready") {
		   cout << "disbatch? [y/n]: ";
		   cin >> action;
		   if (action == 'y')
			   p1.disbatch();
	   } else if (p1.report() == "running") {
		   cout << "suspend, block, or exit? [s/b/e]: ";
		   cin >> action;
		   if (action == 's')
			   p1.suspend();
		   else if (action == 'b')
			   p1.block();
		   else if (action == 'e') {
			   p1.exit();
			   exit = true;
		   }
	   } else if (p1.report() == "blocked") {
		   cout << "unblock? [y/n]: ";
		   cin >> action;
		   if (action == 'y')
			   p1.unblock();
	   }
   }

   
   // demonstrates that two Processes
   // may be in two different states
	Process p2;
	cout << "Process " << p2.getID() << " is " <<  p2.report() << endl;
}
