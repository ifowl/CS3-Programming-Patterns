//Lab 6 - Part 1: Orange selection using multimap
//Isaac Fowler
//
//oranges.cpp

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector;
using std::multimap;
using std::make_pair;

enum class Variety {orange, pear, apple};
vector<string> colors = {"red", "green", "yellow"};

int main(){
   srand(time(nullptr));
   multimap<Variety,string> tree;

   int treeSize = rand()%100 + 1;

   for(int i = 0; i < treeSize; ++i) {
           tree.insert(make_pair(static_cast<Variety>(rand()%3), colors[rand()%3])); //insert random fruit and color into multimap
   }

   cout << "Colors of the oranges: ";
   for(auto fruit : tree) {
        if(fruit.first == Variety::orange) cout << fruit.second << ", "; //print fruit color of oranges only
   }
   cout << endl;
}
