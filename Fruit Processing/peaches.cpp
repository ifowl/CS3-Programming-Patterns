//Lab 6 - Part 3: Peach jamming
//Isaac Fowler
//
//peaches.cpp

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;
using namespace std::placeholders;

struct Peaches{
   double weight; // oz
   bool ripe;  // ripe or not
   void print() const { cout << (ripe?"ripe":"green") << ", " <<  weight << endl; }
};

double createJam(double &jamWeight, Peaches p, double maxWeight) { //functor for practicing binding
        if (p.weight < maxWeight) return (jamWeight += p.weight);
        else return jamWeight;
}




int main(){
        srand(time(nullptr));
        const double minWeight = 8.;
        const double maxWeight = 3.;

        cout << "Input basket size: ";
        int size;
        cin >> size;

        vector <Peaches> basket(size);

        // assign random weight and ripeness peaches in the basket
        // replace with generate()
        std::generate(basket.begin(), basket.end(), [minWeight, maxWeight](){
                  Peaches p;
                  p.weight = minWeight + static_cast<double>(rand())/RAND_MAX*(maxWeight - minWeight);
                  p.ripe = rand() % 2;
                  return p;
                  });

        // for_each() possibly
        cout << "all peaches"<< endl;
        for_each(basket.begin(), basket.end(), [](Peaches p){ p.print(); });

        // moving all the ripe peaches from basket to peck
        // remove_copy_if() with back_inserter()/front_inserter() or equivalents
        deque<Peaches> peck;

        //This function is named poorly. it doesn't actually remove from the vector. It only copies into the peck according to my lambda predicate
        std::remove_copy_if(basket.begin(), basket.end(), std::front_inserter(peck), [](Peaches p){ return !p.ripe; });
        //Now actually remove ripe peaches from basket
        auto newEnd = std::remove_if(basket.begin(), basket.end(), [](Peaches p){ return p.ripe; });
        basket.erase(newEnd, basket.end());

        // for_each() possibly
        cout << "peaches remainng in the basket"<< endl;
        for_each(basket.begin(), basket.end(), [](Peaches p){ p.print(); });
        cout << endl;

        // for_each() possibly
        cout << "peaches moved to the peck"<< endl;
        for_each(peck.begin(), peck.end(), [](Peaches p){ p.print(); });


        // prints every "space" peach in the peck
        const int space=3;
        cout << "\nevery " << space;
        if (space == 2) cout << "nd";
        else if (space == 3) cout << "rd";
        else if (space > 3) cout << "th";
        cout << " peach in the peck"<< endl;

        // replace with advance()/next()/distance()
        // no iterator arithmetic
        auto it=peck.cbegin()-1;
        while(it < peck.cend()){
                std::advance(it, space);
                if (it >= peck.end()) break;
                else it->print();
        }

        // putting all small ripe peaches in a jam
        // use a binder to create a functor with configurable max weight
        // accumulate() or count_if() then remove_if()
        const double weightToJam = 10.0;
        double jamWeight = 0;
        //This takes the createJam functor and binds weightToJam to the 3rd parameter.
        auto binder = std::bind(createJam, _1, _2, weightToJam);

        jamWeight = std::accumulate(peck.begin(), peck.end(), 0.0, binder);

        cout << "Weight of jam is: " << jamWeight << endl;

        auto newEnd2 = std::remove_if(peck.begin(), peck.end(), [weightToJam](Peaches p) { return p.weight < weightToJam; });
        peck.erase(newEnd2, peck.end());

}
