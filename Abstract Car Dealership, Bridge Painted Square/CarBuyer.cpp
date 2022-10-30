// Lab 9: Part One - Abstract Factory Design Pattern. I modiifed the behavior to work with a vector of cars in the CarLot.
// Isaac Fowler
//
// CarBuyer.cpp

#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include "CarFactory.hpp"

using std::vector;
using std::cout; using std::endl;

class CarLot{
public:
   	CarLot();
   	Car *testDriveCar(){return cars4Sale_[carIndex];}

   	// if a car is bought, requests a new one
   	Car *buyCar(){ 
      		Car *bought = cars4Sale_[carIndex];
      		cars4Sale_[carIndex] = factories_[rand()%factories_.size()]->requestCar(); //replace bought car
      		return bought;
   	}
	Car *nextCar();
	int lotSize() { return lotSize_; }
	void printLot();
		     
private:
   	//Car *car4sale_; // single car for sale at the lot
   	vector<Car*> cars4Sale_; //initialize these cars in the CarLot constructor
   	vector<CarFactory *> factories_;
	const int lotSize_ = 10; //amount of cars in lot
	int carIndex = 0;
};

Car* CarLot::nextCar() {
	carIndex++;
	if (carIndex >= lotSize_)
		carIndex = 0;
	return cars4Sale_[carIndex];
}

void CarLot::printLot() {
	cout << "The Car Lot contains the cars:" << endl;
	for (int i = 0; i < lotSize_; ++i) {
		cout << cars4Sale_[i]->getMake() << " " << cars4Sale_[i]->getModel();
		if (i < lotSize_-1)
			cout << ", ";
	}
	cout << endl;
}

CarLot::CarLot(){
   	// creates 2 Ford factories and 2 Toyota factories 
   	factories_.push_back(new FordFactory());   
   	factories_.push_back(new ToyotaFactory());
   	factories_.push_back(new FordFactory());
   	factories_.push_back(new ToyotaFactory());

   	// gets the first car for sale (initialize first value)
	for (int i = 0; i < lotSize_; ++i) {
		auto newCar = factories_[rand() % factories_.size()] -> requestCar();
		cars4Sale_.push_back(newCar);
	}
   	//car4sale_ = factories_[rand() % factories_.size()] -> requestCar();
}



CarLot *carLotPtr = nullptr; // global pointer instantiation

// test-drives a car
// buys it if Toyota
void toyotaLover(int id){
   	if (carLotPtr == nullptr) {
      		carLotPtr = new CarLot();
		carLotPtr -> printLot();
	}
	
   	static const std::array<std::string, 5> models = {"Corolla", "Camry", "Prius", "4Runner", "Yaris"};
	std::string carDesired = models[rand() % models.size()];

   	Car *toBuy = carLotPtr -> testDriveCar();
	cout << "Jill Toyoter " << id << " is looking for a " << carDesired << endl;
	for (int i = 0; i < carLotPtr->lotSize(); ++i) {
   		cout << "  test driving " << toBuy->getMake() << " " << toBuy->getModel();

   		if (toBuy->getMake() == "Toyota" && toBuy->getModel() == carDesired){
      			cout << " - love it! buying it!" << endl;
      			carLotPtr -> buyCar();
			break;
   		} else
      			cout << " - did not like it!" << endl;
		toBuy = carLotPtr -> nextCar();
	}
}

// test-drives a car
// buys it if Ford
void fordLover(int id){
   	if (carLotPtr == nullptr) {
      		carLotPtr = new CarLot();
		carLotPtr -> printLot();
	}

	static const std::array<std::string, 4> models = {"Focus", "Mustang", "Explorer", "F-150"};
	std::string carDesired = models[rand() % models.size()];

   	Car *toBuy = carLotPtr -> testDriveCar();
   	cout << "Jack Fordman " << id << " is looking for a " << carDesired <<  endl;
	for (int i = 0; i < carLotPtr->lotSize(); ++i) {
   		cout << "  test driving " << toBuy->getMake() << " " << toBuy->getModel();
   
   		if (toBuy->getMake() == "Ford" && toBuy->getModel() == carDesired) {
      			cout << " - love it! buying it!" << endl;
      			carLotPtr -> buyCar();
			break;
	   	} else
      			cout << " - did not like it!" << endl;
		toBuy = carLotPtr -> nextCar();
	}
}

int main() {
   	srand(time(nullptr));
	
   	const int numBuyers=10;
   	for(int i=0; i < numBuyers; ++i)
      		if(rand()% 2 == 0)
	 		toyotaLover(i);
      		else
	 		fordLover(i);
  
}
