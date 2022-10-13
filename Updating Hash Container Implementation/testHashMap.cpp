//Lab 7 - testing custom hashmap container
//Isaac Fowler
//
//testHashMap.cpp

#include "hashmap.hpp"
#include <iostream>
#include <string>

using std::cin; using std::cout; using std::endl; using std::string;

int main() {
	hashmap<int, int> hm;

	//Safe insertion implemented. insert() returns a pair<pointer, bool>
	cout << endl << "Safe insertion:" << endl;
	auto result = hm.insert(make_pair(1, 100));
	
	cout << "Inserting: Key 1, Value 100" << endl;
	cout << "Pointer returned: Key " << (result.first)->first << ", Value " << (result.first)->second << endl;
	if (result.second) cout << "-> element was inserted successfully." << endl;
	else cout << "-> key taken. element was not inserted" << endl;

	result = hm.insert(make_pair(2, 200));
	cout << "Inserting: Key 2, Value 200" << endl;
        cout << "Pointer returned: Key " << (result.first)->first << ", Value " << (result.first)->second << endl;
        if (result.second) cout << "-> element was inserted successfully." << endl;
        else cout << "-> key taken. element was not inserted" << endl;

	result = hm.insert(make_pair(1, 234));
	cout << "Inserting: Key 1, Value 234" << endl;
	cout << "Pointer returned: Key " << (result.first)->first << ", Value " << (result.first)->second << endl;
        if (result.second) cout << "-> element was inserted successfully." << endl;
        else cout << "-> key taken. element was not inserted" << endl;

	//Indexing operator is more efficient
	cout << endl;
	cout << "Indexing operator:" << endl;

	hm[3] = 25;
	cout << "Inserting: Key 3, Value 25" << endl;
	cout << "Checking value of key [3]: " << hm[3] << endl;

	//erase() function also returns a pair<pointer, bool>
	cout << endl << "Erasing:" << endl;
	result = hm.erase(2);
	cout << "Deleting: Key 2" << endl;
	if (result.first == nullptr) cout << "Pointer returned: nullptr (because it removed the last element of container)" << endl;
	else cout << "Pointer returned: Key " << (result.first)->first << ", Value " << (result.first)->second << endl;
	if (result.second) cout << "-> element was erased successfully." << endl;
	else cout << "-> that key does not exist" << endl;

	result = hm.erase(3);
        cout << "Deleting: Key 3" << endl;
        if (result.first == nullptr) cout << "Pointer returned: nullptr (because it removed the last element of container)" << endl;
        else cout << "Pointer returned: Key " << (result.first)->first << ", Value " << (result.first)->second << endl;
        if (result.second) cout << "-> element was erased successfully." << endl;
        else cout << "-> that key does not exist" << endl;

	result = hm.erase(2);
        cout << "Deleting: Key 2" << endl;
        if (result.first == nullptr) cout << "Pointer returned: nullptr (because the key does not exist)" << endl;
        else cout << "Pointer returned: Key " << (result.first)->first << ", Value " << (result.first)->second << endl;
        if (result.second) cout << "-> element was erased successfully." << endl;
        else cout << "-> that key does not exist" << endl;

	//rehash() function changes the number of buckets and re-arranges the elements
	cout << endl << "Rehashing:" << endl;
	cout << "Repopulating hash map..." << endl;
	hm[2] = 22;
	hm[3] = 33;
	hm[4] = 44;
	cout << "Inserting: <2, 22> <3, 33> <4, 44>" << endl;
	cout << "Current number of buckets is: " << hm.getNumBuckets() << endl;
	hm.rehash(150);
	cout << "Rehashing..." << endl;
	cout << "New number of buckets is: " << hm.getNumBuckets() << endl;
	cout << "Testing new hashed values:" << endl;
	cout << "hm[1] is " << hm[1] << endl;
	cout << "hm[2] is " << hm[2] << endl;
	cout << "hm[3] is " << hm[3] << endl;
	cout << endl << "Testing complete!" << endl << endl;
}
