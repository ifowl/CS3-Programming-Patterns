// Lab 10: Part 3 - Prototype design pattern
// Isaac Fowler
//
// PrototypeScheduler.cpp


#include <iostream>
#include <string>
#include <queue>
using std::cin; using std::cout; using std::endl; using std::string;
using std::queue;

class State;

// context
class Process{
public:
   Process();
   Process(State *s): state_(s) { //allow cloning of Process with exact state
	   ID_ = nextID_++;
   }

   // behaviors
   void dispatch();
   void block();
   void unblock();
   void suspend();
   void exit();
   string report();

   int getID() const { return ID_; }
   // part of implementation of state pattern
   void changeState(State* state){state_=state;}
   Process *clone() {
	   Process *p = new Process(state_);
	   return p;
   }
private:
   State* state_;
   static int nextID_;
   int ID_;
   bool killed_ = false;
};

// absract state
class State{
public:
   // provides default implementation
   virtual void dispatch(Process*){} 
   virtual void suspend(Process*){}
   virtual void block(Process*){}
   virtual void unblock(Process*){}
   virtual void exit(Process*){}
   virtual string report() =0;
   void changeState(Process* p, State* s){
      p->changeState(s);
   }
   //int getID(Process* p){
	//   return p->getID();
   //}
};

class Ready: public State{
public:
   static State* instance(){
      static State* onlyInstance = new Ready;
      return onlyInstance;
   }
   void dispatch(Process*) override;  
   string report() override {
	   return "READY";
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
   string report() override {return "RUNNING";}
};


class Blocked: public State{
public:
   static State* instance(){
      static State* onlyInstance = new Blocked;
      return onlyInstance;
   }
   void unblock(Process*) override;
   string report() override {return "BLOCKED";}
};


// state transitions member functions

void Ready::dispatch(Process *p) {
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
int Process::nextID_ = 0;

// handles/behaviors
void Process::dispatch(){state_->dispatch(this);}
void Process::suspend(){state_->suspend(this);}
void Process::block(){state_->block(this);}
void Process::unblock(){state_->unblock(this);}
void Process::exit(){killed_ = true;}
string Process::report(){return state_->report();}

int main(){
	srand(time(nullptr));
	Process prototype;

	queue<Process*> readyQ;
	queue<Process*> blockedQ;

	for (int i = 0; i < 4; ++i) {
		Process *temp = prototype.clone();
		cout << "PID " << temp->getID() << ": created" << endl;
	        cout << "PID " << temp->getID()	<< " state: " << temp->report() << endl;
		readyQ.push(temp);
	}
	cout << endl << "---------- scheduler running -------------" << endl;

	int stateTransition;
	while (!readyQ.empty() || !blockedQ.empty()) {
		while (!readyQ.empty()) {
			auto *proc = readyQ.front();
			stateTransition = rand() % 3;
			proc->dispatch();
			cout << "Dispatching..." << endl;
			cout << "PID " << proc->getID() << " state: " << proc->report() << endl;
			//manage running process change state
			if (stateTransition == 0) { //exit
				proc->exit();
				cout << "Exiting..." << endl;
				cout << "PID " << proc->getID() << ": exited" << endl << endl;
			} else if (stateTransition == 1) { //suspend
				proc->suspend();
				readyQ.push(proc);
				cout << "Suspending..." << endl;
				cout << "PID " << proc->getID() << " state: " << proc->report() << endl << endl;
			} else if (stateTransition == 2) { //block
				proc->block();
				blockedQ.push(proc);
				cout << "Blocking..." << endl;
				cout << "PID " << proc->getID() << " state: " << proc->report() << endl << endl;
			}
			readyQ.pop();
		}

		//manage blocked queue
		int blockedNum = rand() % 2;
		if (!blockedQ.empty() && blockedNum == 0) {
			cout << "The ready queue is empty." << endl << endl;
			auto *blockproc = blockedQ.front();
			blockproc->unblock();
			readyQ.push(blockproc);
			cout << "Unblocking..." << endl;
			cout << "PID " << blockproc->getID() << " state: " << blockproc->report() << endl << endl;
			blockedQ.pop();
		}
	}

	cout << "all processes exited." << endl << "---------- scheduler done -------------" << endl;
}
