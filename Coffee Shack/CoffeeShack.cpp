// Lab 11 - Coffee Shack using decorator, chain of responsibility, and observer design patterns
// Isaac Fowler
//
// CoffeeShack.cpp

#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>

using std::cout; using std::cin; using std::endl; using std::string;
using std::unordered_set; using std::vector;

enum class DrinkType { small, medium, large };

//Component (to be decorated)
class Drink {
public:
    Drink(DrinkType type = DrinkType::small, int price = 0) :
        type_(type), price_(price) {}
    virtual int getPrice() const { return price_; }
    virtual string getName() const {
        string name;
        if (type_ == DrinkType::small)
            name = "small coffee with ";
        else if (type_ == DrinkType::medium)
            name = "medium coffee with ";
        else if (type_ == DrinkType::large)
            name = "large coffee with ";
        return name;
    }
private:
    int price_;
    DrinkType type_;
};

//Decorator #1
class Sugar : public Drink {
public:
    Sugar(const Drink* wrapped) : wrapped_(wrapped) {}
    int getPrice() const override {
        return wrapped_->getPrice() + 1;
    }
    string getName() const override {
        return wrapped_->getName() + "sugar, ";
    }
private:
    const Drink* wrapped_;
};

//Decorator #2
class Cream : public Drink {
public:
    Cream(const Drink* wrapped) : wrapped_(wrapped) {}
    int getPrice() const override {
        return wrapped_->getPrice() + 2;
    }
    string getName() const override {
        return wrapped_->getName() + "cream, ";
    }
private:
    const Drink* wrapped_;
};

//Decorator #3
class MilkFoam : public Drink {
public:
    MilkFoam(const Drink* wrapped) : wrapped_(wrapped) {}
    int getPrice() const override {
        return wrapped_->getPrice() + 3;
    }
    string getName() const override {
        return wrapped_->getName() + "milk foam, ";
    }
private:
    const Drink* wrapped_;
};

//Abstract class for chain of responsibility
//Subject that notifies customer observers.
class Barista {
public:
    Barista(Barista* successor = nullptr) :successor_(successor) {}
    virtual void handleRequest(string drinkName) {
        if (successor_ != nullptr)
            successor_->handleRequest(drinkName);
    }
    string getCustomerName() const;
    string getCustomerOrder() const;
    void registerObserver(class Customer* o) { orders_.push_back(o); }
    void deregisterObserver() {
        orders_.erase(orders_.begin() + orderIndex_);
    }
    void notifyObservers() const;
    void finishOrder();
protected:
    vector<Customer*> orders_;
    int orderIndex_;
    //vector<class Customer *> orders_;
private:
    Barista* successor_;
};

//Observer that registers with a barista and receives notifications.
class Customer
{
public:
    Customer(const string& name, Drink* drink, Barista* subject) : name_(name), subject_(subject), drink_(drink) {
        subject_->registerObserver(this);
    }
    void notify() const {
        cout << "Notifying " << name_ << ": " << subject_->getCustomerOrder() << endl;
        if (name_ == subject_->getCustomerName()) {
            cout << "*" << name_ << " picks up their drink*" << endl;
        }
    }
    string getName() const { return name_; }
    Drink* getDrink() const { return drink_; }
private:
    std::string name_;
    Drink* drink_;
    class Barista* subject_;
};

//Barista member functions
string Barista::getCustomerName() const {
    return orders_[orderIndex_]->getName();
}
string Barista::getCustomerOrder() const {
    return (orders_[orderIndex_]->getName() + "'s order of " + orders_[orderIndex_]->getDrink()->getName() + "is ready. It will cost $" + std::to_string(orders_[orderIndex_]->getDrink()->getPrice()));
}
void Barista::notifyObservers() const { for (auto e : orders_) e->notify(); }
void Barista::finishOrder() {
    orderIndex_ = rand() % orders_.size(); //Orders aren't completed FIFO. They are random.
    this->notifyObservers();
    this->deregisterObserver();
}

//derived Barista class #1
class JuniorBarista : public Barista {
public:
    JuniorBarista(Barista* successor = nullptr) : Barista(successor) {}
    void handleRequest(string drinkName) override {
        if (drinkName.find("sugar") == string::npos && drinkName.find("cream") == string::npos && drinkName.find("foam") == string::npos) {
            cout << "A Junior Barista will prepare your coffee." << endl;
        }
        else
            return Barista::handleRequest(drinkName);
    }
};

class SeniorBarista : public Barista {
public:
    SeniorBarista(Barista* successor = nullptr) : Barista(successor) {}
    void handleRequest(string drinkName) override {
        if (drinkName.find("foam") == string::npos) {
            cout << "A Senior Barista will prepare your coffee." << endl;
        }
        else
            return Barista::handleRequest(drinkName);
    }
};

class Manager : public Barista {
public:
    Manager(Barista* successor = nullptr) : Barista(successor) {}
    void handleRequest(string drinkName) override {
        cout << "A Manager will prepare your coffee." << endl;
    }
};


int main() {
    srand(time(nullptr));

    int numCust; int ordersCompleted = 0;
    cout << "How many customers would you like to simulate? ";
    cin >> numCust;

    //Get a chain of Baristas.
    Barista* bp = new JuniorBarista(new SeniorBarista(new Manager));

    for (int i = 0; i < numCust; ++i) {
        cout << endl << "     Customer #" << i + 1 << endl;
        char size;
        cout << "Welcome to Coffee Shack, can I get you [l]arge, [m]edium, or [s]mall coffee? ";
        cin >> size;
        DrinkType type; int price;
        if (size == 's') {
            type = DrinkType::small;
            price = 5;
        }
        else if (size == 'm') {
            type = DrinkType::medium;
            price = 7;
        }
        else if (size == 'l') {
            type = DrinkType::large;
            price = 10;
        }
        Drink* customerCoffee;
        customerCoffee = new Drink(type, price);
        char ingredient;
        while (ingredient != 'd') {
            cout << "Would you like to add [s]ugar, [c]ream, milk [f]oam, or [d]one? ";
            cin >> ingredient;
            if (ingredient == 's')
                customerCoffee = new Sugar(customerCoffee);
            else if (ingredient == 'c')
                customerCoffee = new Cream(customerCoffee);
            else if (ingredient == 'f')
                customerCoffee = new MilkFoam(customerCoffee);
        }
        ingredient = 'o';
        cout << "Can I get your name? ";
        string customerName;
        cin >> customerName;

        bp->handleRequest(customerCoffee->getName());

        Customer* newCustomer = new Customer(customerName, customerCoffee, bp);

        if (rand() % 3 == 0) { //33% chance of an order being finished
            bp->finishOrder();
            ++ordersCompleted;
        }
    }
    while (ordersCompleted < numCust) { //just in case there are customers still waiting for their coffee
        bp->finishOrder();
        ++ordersCompleted;
    }
}
