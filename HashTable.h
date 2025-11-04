/**
 * Bryce Fox - Project 4
 * CS3100
 * 11/3/2025
 *
 * HashTable.h
 * Defines the HashTable and HashTableBucket classes
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <optional> // Required for returning optional values from get()

using namespace std;

// Enum defining the three possible states of a hash table bucket.
enum class BucketType {
    NORMAL, // Contains valid key-value data.
    ESS,    // Empty Since Start: never used.
    EAR     // Empty After Remove: previously contained data, now logically deleted.
};

// Represents a single storage unit within the hash table.
class HashTableBucket {
public:

    string key;          // The key string.
    int value;           // The associated integer value.
    BucketType type;     // The state of the bucket (NORMAL, ESS, EAR).

    // Initializes type to ESS.
    HashTableBucket();
    // Initializes with data, setting type to NORMAL.
    HashTableBucket(string key, int value);
    // Loads key-value data into the bucket.
    void load(string key, int value);
    // Returns true if the bucket is available for insertion (ESS or EAR).
    bool isEmpty() const;

    // Stream output operator for displaying the bucket's content.
    friend ostream& operator<<(ostream& os, const HashTableBucket& bucket);

};

// Implements the main hash table structure.
class HashTable {
public:

    // Default capacity for initialization.
    static constexpr size_t DEFAULT_INITIAL_CAPACITY = 8;
    // Constructor; initializes the table structure.
    HashTable(size_t initCapacity = DEFAULT_INITIAL_CAPACITY);

    // Core Mutators and Accessors
    bool insert(string key, size_t value);
    bool remove(string key);
    bool contains(const string& key) const;
    // Retrieves value. Returns optional<int> to handle key absence.
    optional<int> get(const string& key) const;
    // Subscript operator. Returns reference to value (potential UB if key not found).
    int& operator[](const string& key);
    // Returns a vector containing all keys in the table.
    vector<string> keys() const;

    // Status Metrics
    double alpha() const;     // Calculates and returns the load factor (size/capacity).
    size_t capacity() const;  // Returns the total bucket count.
    size_t size() const;      // Returns the number of stored elements.

    // Stream output operator for displaying the entire table.
    friend ostream& operator<<(ostream& os, const HashTable& hashTable);

private:
    vector<HashTableBucket> tableData; // The underlying vector of buckets.
    vector<size_t> offsets;           // Randomized offsets for the probe sequence.
    size_t currentSize = 0;           // Current element count.
    size_t currentCapacity = 0;       // Current size of the tableData vector.

    // Maintenance functions
    void resize();        // Doubles capacity and rehashes elements.
    void generateOffsets(); // Creates the random probe sequence permutation.

};

#endif