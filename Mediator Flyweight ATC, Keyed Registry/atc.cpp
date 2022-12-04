// Lab 13: Part One - Modify code to allow only one Logo instance per Airline using Flyweight design pattern
// Isaac Fowler
//
// atc.cpp (air traffic control)


#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
#include <algorithm>

// for timed sleep
#include <thread>
#include <chrono>


using std::cout; using std::cin; using std::endl;
using std::string;

//abstract flyweight
class AirlineLogo {
public:
	virtual string image() = 0;
};

//concrete flyweights
class AirFrance: public AirlineLogo {
public:
	string image() override { return "AIRFRANCE"; }
};
class KLM: public AirlineLogo {
public:
	string image() override { return "KLM"; }
};
class Aeroflot: public AirlineLogo {
public:
	string image() override { return "AEROFLOT"; }
};
class Lufthansa: public AirlineLogo {
public:
	string image() override { return "LUFTHANSA"; }
};
class Delta: public AirlineLogo {
public:
	string image() override { return "DELTA"; }
};
class United: public AirlineLogo {
public:
	string image() override { return "UNITED"; }
};
class JetBlue: public AirlineLogo {
public:
	string image() override { return "JETBLUE"; }
};

//flyweight factory
class Airline {
public:
	static AirlineLogo *makeAirline(int i) {
		if (airlines_.find(i) == airlines_.end()) {
			switch (i) {
				case 0:
					airlines_[i] = new AirFrance; break;
				case 1:
					airlines_[i] = new KLM; break;
				case 2:
					airlines_[i] = new Aeroflot; break;
				case 3:
					airlines_[i] = new Lufthansa; break;
				case 4:
					airlines_[i] = new Delta; break;
				case 5:
					airlines_[i] = new United; break;
				case 6:
					airlines_[i] = new JetBlue; break;
			}
		}
		return airlines_[i];
	}

private:
	static std::map<int, AirlineLogo*> airlines_;
};

std::map<int, AirlineLogo*> Airline::airlines_;

// abstract mediator
class Controller{
public:
   virtual void join(class Flight*)=0;
   virtual void leave(class Flight*)=0;
   virtual void broadcast() = 0;
   virtual void observe() = 0;
};


// concrete mediator
// tower, runway clearance
class Tower: public Controller{
public:
   void join(class Flight*) override; 
   void leave(class Flight*) override;
   void broadcast() override;
   void observe() override;
private:
   std::set<class Flight*> waiting_;
};


// abstract colleague
class Flight {
public:
   Flight(Controller *controller):controller_(controller), status_(Status::waiting){
      task_ = rand() % 2 ? Task::taxiing : Task::approaching;
      controller_->join(this);
   }
   void receive(const string &msg){
      if (msg.find(flightName_) != string::npos || msg.find("all") != string::npos){
	 if(msg.find("clear") != string::npos){
	    cout << flightName_ << " roger that, ";
	    cout << (task_ == Task::taxiing ? "taking off" : "landing") << endl;
	    status_ = Status::cleared;
	 } else if(msg.find("status") != string::npos) {
	    cout << flightName_
		 << (status_ == Status::waiting ? " waiting to " : " cleared to ")   
	         << (task_ == Task::taxiing ? "take off" : "land") << endl;
	 } else
	    cout << "Tower, this is " << flightName_ << " please repeat." << endl;
      }
   }
   bool isCleared() const {return status_ == Status::cleared;}
   void proceed(){
      std::this_thread::sleep_for(std::chrono::seconds(rand()%3+1)); // wait a while    
      cout << "..." << airline_->image() << flightNum_ << (task_ == Task::taxiing ? " took off" : " landed") << "..." << endl;
      controller_->leave(this);
   }

protected:
   AirlineLogo *airline_;
   int flightNum_;
   string flightName_;
   Controller *controller_;
   enum class Task {taxiing, approaching};  Task task_;
   enum class Status {waiting, cleared};    Status status_;
};

// concrete colleagues
class Airbus: public Flight{
public:
   Airbus(Tower *tower): Flight(tower){
	   int index = rand()%4;
	   flightNum_ = rand()%1000;
	   airline_ = Airline::makeAirline(index);
      static const std::vector<string> companies = {"AirFrance", "KLM", "Aeroflot", "Lufthansa"};
      flightName_ = companies[index] + std::to_string(flightNum_);
      cout << flightName_ << " requesting " << (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
   }
};

class Boeing: public Flight{
public:
   Boeing(Tower *tower): Flight(tower){
	   int index = rand()%3;
	   flightNum_ = rand()%1000;
	   airline_ = Airline::makeAirline(index+4);
      static const std::vector<string> companies = {"Delta", "United", "JetBlue"};
      flightName_ = companies[index] + std::to_string(flightNum_);
      cout << flightName_ << " requesting " << (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
   }
};


// member functions for Tower
void Tower::broadcast() {
   cout << "Tower: ";
   string msg;
   getline(cin,msg);
   if(!msg.empty())
      for(auto f: waiting_) f->receive(msg);
}


void Tower::observe() {
   auto findCleared = [](Flight *f){return f->isCleared();};
   
   auto toProceed = std::find_if(waiting_.begin(),waiting_.end(), findCleared);
   
   while (toProceed != waiting_.end()){ // found a cleared flight
      (*toProceed) -> proceed();
      toProceed = std::find_if(waiting_.begin(), waiting_.end(), findCleared);
      if(toProceed != waiting_.end())
	 cout << "MAYDAY! MAYDAY! MAYDAY! " << endl; // more than a single flight cleared
   }
}


void Tower::join(Flight *f) {
   waiting_.insert(f);
}


void Tower::leave(Flight *f) {
   waiting_.erase(f);
   delete f;
}

int main(){
   srand(time(nullptr));
   Tower jfk;

   new Boeing(&jfk);
   new Airbus(&jfk);
   new Boeing(&jfk);
   new Airbus(&jfk);

   while(true){
      jfk.broadcast();
      jfk.observe();
      if(rand() % 2){
	 if (rand() % 2)
	    new Boeing(&jfk);
	 else
	    new Airbus(&jfk);
      }
   }
}
