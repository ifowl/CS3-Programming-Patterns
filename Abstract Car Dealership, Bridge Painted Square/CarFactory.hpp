// interface for abstract and concrete car factories
// Mikhail Nesterenko
// 7/16/2014

#include <string>

// abstract product
class Car{
 public:
   std::string getMake() const {return make_;}
   std::string getModel() const {return model_;}
 protected:
   std::string make_; // this car's make
   std::string model_; // this car's model
};

// two concrete products
class Ford : public Car{
 public:
   Ford();
};

class Toyota : public Car{
 public:
   Toyota();
};

// absract factory
// We can't instantiate objects of this class because it is abstract by syntax (virtual method)
class CarFactory {
public:
   CarFactory(std::string location, int numCarsInProduction):
      location_(location), 
      numCarsInProduction_(numCarsInProduction){}

   Car* requestCar();
   int getNumCarsInProduction() const {return numCarsInProduction_;}
   std::string getLocation() const {return location_;}
   virtual ~CarFactory(){}
protected:
   virtual Car* makeCar() =0;

private:
   int numCarsInProduction_;
   std::string location_;
};

// two concrete factories
class FordFactory : public CarFactory {
public:
   FordFactory(std::string location="", int numCarsInProduction=0):
       CarFactory(location, numCarsInProduction){} //calling constructor of base class
protected:
   Car* makeCar() override;
};

class ToyotaFactory : public CarFactory {
public:
   ToyotaFactory (std::string location="", 
		  int numCarsInProduction=0):
       CarFactory(location, numCarsInProduction){} //calling constructor of base class
protected:
   Car* makeCar() override;
};