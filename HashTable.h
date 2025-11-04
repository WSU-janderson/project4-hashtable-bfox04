/**
 * HashTable.h
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <optional>

using namespace std;

enum class BucketType { NORMAL, ESS, EAR };

class HashTableBucket {
public:

    string key;
    int value;
    BucketType type;

    HashTableBucket();
    HashTableBucket(string key, int value);
    void load(string key, int value);
    bool isEmpty() const;

    friend ostream& operator<<(ostream& os, const HashTableBucket& bucket);

};

class HashTable {
public:

    static constexpr size_t DEFAULT_INITIAL_CAPACITY = 8; // add this due to build failure w/ test harness
    HashTable(size_t initCapacity = DEFAULT_INITIAL_CAPACITY);

    bool insert(string key, size_t value);
    bool remove(string key);
    bool contains(const string& key) const;
    optional<int> get(const string& key) const;
    int& operator[](const string& key);
    vector<string> keys() const;
    double alpha() const;
    size_t capacity() const;
    size_t size() const;

    friend ostream& operator<<(ostream& os, const HashTable& hashTable);

private:
    vector<HashTableBucket> tableData;
    vector<size_t> offsets;
    size_t currentSize = 0;
    size_t currentCapacity = 0;

    void resize();
    void generateOffsets();

};

#endif
