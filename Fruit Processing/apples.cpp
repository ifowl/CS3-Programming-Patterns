//Lab 6 - Part 2: Sorting apples using STL algorithms
//Isaac Fowler
//
//apples.cpp

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;

struct Apples{
        double weight; // oz
        string color;  // red or green
        void print() const { cout << color << ", " <<  weight << endl; }
};

Apples generateCrate() {
        Apples apple;
        const double minWeight = 8.;
        const double maxWeight = 3.;
        apple.weight = minWeight + static_cast<double>(rand())/RAND_MAX*(maxWeight - minWeight);
        apple.color = rand() % 2 == 1 ? "green" : "red";
        return apple;
}

int main(){
        srand(time(nullptr));

        cout << "Input crate size: ";
        int size;
        cin >> size;

        vector <Apples> crate(size);

        // assign random weight and color to apples in the crate
        // replace with generate()
        std::generate(crate.begin(), crate.end(), generateCrate); //Could also use lambda function. Just wanted to show another way of using callback.


        // for_each() possibly
        cout << "all apples"<< endl;
        std::for_each(crate.begin(), crate.end(), [](Apples a){a.print();});


        cout << "Enter weight to find: ";
        double toFind;
        cin >> toFind;

        // count_if()
        int cnt = count_if(crate.begin(), crate.end(), [toFind](Apples a){return (a.weight > toFind);});
        cout << "There are " << cnt << " apples heavier than " << toFind << " oz" <<  endl;

        // find_if()
        auto it = crate.begin();
        cout << "at positions ";
        for(int i=0; i < cnt; ++i) {
                it = std::find_if(it, crate.end(), [toFind](Apples a) { return (a.weight > toFind);});
                cout << it - crate.begin() << ", ";
                ++it;
        }
        cout << endl;


        // max_element()
        auto maxIt = std::max_element(crate.begin(), crate.end(), [](Apples a, Apples b) { return (a.weight < b.weight); });
        cout << "Heaviest apple weighs: " << maxIt->weight << " oz" << endl;


        // for_each() or accumulate()
        double sum = 0;
        auto totalSum = std::accumulate(crate.begin(), crate.end(), 0.0, [](double &sum, Apples a) {return (sum += a.weight);});
        cout << "Total apple weight is: " << totalSum << " oz" << endl;


        // transform()
        cout << "How much should they grow: ";
        double toGrow;
        cin >> toGrow;

        //No overloaded = operator to allow incrementing weight.
        //Therefore it's necessary create new apple with incremented weight to return for transform
        std::transform(crate.begin(), crate.end(), crate.begin(), [toGrow](Apples a) {
                        Apples temp;
                        temp.color = a.color;
                        temp.weight = (a.weight += toGrow);
                        return temp;
                        });
        // remove_if()
        cout << "Input minimum acceptable weight: ";
        double minAccept;
        cin >> minAccept;

        //remove_if re-arranges the elements of the vector so that the ones we want to keep are in the range (vec.begin(), return_iterator). So you must erase everything after the new iterator (new end)
        //This is called the erase-remove idiom
        auto newEnd = std::remove_if(crate.begin(), crate.end(), [minAccept](Apples a){return (a.weight < minAccept);});
        crate.erase(newEnd, crate.end());
        cout << "removed " << size - crate.size() << " elements" << endl;


        // bubble sort, replace with sort()
        std::sort(crate.begin(), crate.end(), [](Apples a, Apples b){return (a.weight < b.weight);});


        // for_each() possibly
        cout << "sorted remaining apples"<< endl;
        for_each(crate.begin(), crate.end(), [](Apples a){ a.print(); });

}
