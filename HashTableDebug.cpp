/**
 * HashTableDebug.cpp
 * Write your tests in this file
 *
 * testing things ....
 *
 */

#include "HashTable.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    cout << "STARTING TESTS" << endl;

    HashTable ht;
    cout << "Size: " << ht.size() << ", Cap: " << ht.capacity() << ", Alpha: " << ht.alpha() << endl;
    cout << ht << endl;

    ht.insert("apple", 10);
    ht.insert("banana", 20);
    ht.insert("cherry", 30);
    ht.insert("date", 40);
    ht.insert("elderberry", 50);
    cout << "Size: " << ht.size() << ", Cap: " << ht.capacity() << ", Alpha: " << ht.alpha() << endl;
    cout << ht << endl;

    cout << "banana: " << (ht.contains("banana") ? "T" : "F") << endl;
    cout << "grape: " << (ht.contains("grape") ? "T" : "F") << endl;

    optional<int> banana_val = ht.get("banana");
    cout << "Get banana: " << (banana_val.has_value() ? to_string(banana_val.value()) : "nullopt") << endl;

    cout << "Remove cherry: " << (ht.remove("cherry") ? "S" : "F") << endl;
    cout << "Remove fig: " << (ht.remove("fig") ? "S" : "F") << endl;
    cout << "Size: " << ht.size() << ", Cap: " << ht.capacity() << ", Alpha: " << ht.alpha() << endl;
    cout << ht << endl;

    cout << "Insert apple (duplicate): " << (ht.insert("apple", 99) ? "S" : "F") << endl;
    cout << "Size: " << ht.size() << ", Cap: " << ht.capacity() << ", Alpha: " << ht.alpha() << endl;
    cout << ht << endl;

    ht["date"] = 400;
    cout << "Updated date value: " << ht.get("date").value_or(0) << endl;

    cout << "Insert cherry again: " << (ht.insert("cherry", 300) ? "S" : "F") << endl;
    cout << "Size: " << ht.size() << ", Cap: " << ht.capacity() << ", Alpha: " << ht.alpha() << endl;
    cout << ht << endl;

    ht.insert("fuji", 60);
    ht.insert("grape", 70);
    ht.insert("honeydew", 80);
    ht.insert("kiwi", 90);
    ht.insert("lemon", 100);
    ht.insert("mango", 110);
    ht.insert("nectarine", 120);
    ht.insert("orange", 130);
    ht.insert("papaya", 140);
    cout << "Size: " << ht.size() << ", Cap: " << ht.capacity() << ", Alpha: " << ht.alpha() << endl;
    cout << ht << endl;

    HashTableBucket b1("test", 1);
    cout << "B1 (Normal): " << b1 << " (Empty: " << (b1.isEmpty() ? "T" : "F") << ")" << endl;
    HashTableBucket b2;
    cout << "B2 (ESS): " << b2 << " (Empty: " << (b2.isEmpty() ? "T" : "F") << ")" << endl;
    b2.load("loaded", 2);
    cout << "B2 (Loaded): " << b2 << " (Empty: " << (b2.isEmpty() ? "T" : "F") << ")" << endl;

    cout << "\nTESTS COMPLETE" << endl;

    return 0;
}
