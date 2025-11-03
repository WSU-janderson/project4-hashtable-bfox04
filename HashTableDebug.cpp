/**
 * HashTableDebug.cpp
 *
 * Write your tests in this file
 */
#include <iostream>
#include "HashTable.h"

using namespace std;

int main() {
    cout << "Hello, World!" << endl;

    HashTableBucket Bobby("bob",4);

    cout <<  Bobby << endl;
    cout<< Bobby.isEmpty() << endl;

    Bobby.load("Robert",2);

    cout <<  Bobby << endl;
    cout<< Bobby.isEmpty() << endl;

    HashTable roundTable(3);
    HashTable eightSidedTable();



    return 0;
}
