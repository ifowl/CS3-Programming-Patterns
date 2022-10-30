// implementation of carFactory
// Mikhail Nesterenko
// 7/16/2014

#include <array>
#include <cstdlib>
#include "CarFactory.hpp"


// product methods
// Constrcutors for the abstract products
Ford::Ford(){
   static const std::array<std::string, 4>
      models = {"Focus", "Mustang", "Explorer", "F-150"};
   make_ = "Ford";
   model_ = models[rand() % models.size()];
}

Toyota::Toyota(){
   static const std::array<std::string, 5>
      models = {"Corolla", "Camry", "Prius", "4Runner", "Yaris"};
   make_ = "Toyota";
   model_ = models[rand() % models.size()];
}


// factory methods
Car* CarFactory::requestCar() {
	++numCarsInProduction_;
	return makeCar();
}

Car* FordFactory::makeCar() {
	return new Ford(); //return a new instance of the concrete class and point to it
}

Car* ToyotaFactory::makeCar() {
	return new Toyota(); //return a new instance of the conrete class and point to it
}
