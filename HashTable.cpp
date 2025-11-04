/**
 * Bryce Fox - Project 4
 * CS3100
 * 11/3/2025
 *
 * HashTable.cpp
 * Implementation of a Hash Table using open addressing with custom probing sequence.
 */

#include "HashTable.h"

// --- HashTableBucket ---

// Default constructor. Initializes the bucket as Empty Since Start (ESS).
HashTableBucket::HashTableBucket():
    type(BucketType::ESS) {}

// Overloaded constructor. Initializes the bucket with a key-value pair as NORMAL.
HashTableBucket::HashTableBucket(string key, int value):
    key(key), value(value), type(BucketType::NORMAL) {}

// Loads a key-value pair into the bucket, setting the type to NORMAL.
void HashTableBucket::load(string key, int value) {
    this->key = key;
    this->value = value;
    this->type = BucketType::NORMAL;
}

// Checks if the bucket is logically empty (ESS or EAR).
bool HashTableBucket::isEmpty() const {
    return type == BucketType::ESS || type == BucketType::EAR;
}

// Overloads the stream insertion operator for easy printing of bucket contents.
ostream& operator<<(ostream& os, const HashTableBucket& bucket) {
    if (bucket.type == BucketType::NORMAL) {
        // Only output if the bucket holds valid data.
        os << "<" << bucket.key << ", " << bucket.value << ">";
    }
    return os;
}

// --- HashTable ---

// Constructor. Initializes the table with a given capacity.
HashTable::HashTable(size_t initCapacity) {
    int cap_val = initCapacity;
    currentCapacity = cap_val;
    currentSize = 0;
    tableData.resize(currentCapacity); // Allocate space for the table.

    srand(0); // Initialize random seed for reproducibility of offsets.
    generateOffsets(); // Create the random permutation probe sequence.
}

// Inserts a key-value pair into the table. Returns true on success, false on duplicate key.
bool HashTable::insert(std::string key, size_t value) {
    // Check and resize if load factor (alpha) reaches or exceeds the threshold (0.5).
    if (alpha() >= 0.5) {
        resize();
    }

    std::hash<std::string> hasher;
    size_t hash_val = hasher(key);
    size_t homeIndex = hash_val % currentCapacity; // Calculate the initial probe index.

    // insertionIndex tracks the best available empty slot found (EAR preferred over ESS).
    size_t insertionIndex = 0;
    bool foundInsertionSpot = false;

    // Construct the full probe sequence: home index followed by indices offset by the random array.
    std::vector<size_t> probeIndices;
    probeIndices.push_back(homeIndex);
    for(size_t offset : offsets) {
        probeIndices.push_back((homeIndex + offset) % currentCapacity);
    }

    // Iterate through the probe sequence.
    for(size_t idx : probeIndices) {
        HashTableBucket& currentBucket = tableData[idx];

        if (currentBucket.type == BucketType::NORMAL) {
            // Found data. Check for key duplication.
            if (currentBucket.key == key) {
                return false; // Duplicate found, insertion failed.
            }
        } else { // Bucket is ESS (never used) or EAR (deleted).
            // Record the first empty spot found for potential insertion.
            if (!foundInsertionSpot) {
                insertionIndex = idx;
                foundInsertionSpot = true;
            }

            if (currentBucket.type == BucketType::ESS) {
                // ESS terminates the probe sequence. Insert the data into the recorded empty spot.
                tableData[insertionIndex].load(key, value);
                currentSize++;
                return true; // Insertion complete.
            }

            // If EAR, continue probing to check for duplicates that might have been displaced.
        }
    }

    // If the loop completes and an insertion spot was found (implying the sequence was full of EARs), insert.
    if (foundInsertionSpot) {
        tableData[insertionIndex].load(key, value);
        currentSize++;
        return true;
    }

    // Should only be reached if no suitable spot was found after a full probe,
    // which indicates a logic issue if resize worked correctly.
    return false;
}

// Removes a key-value pair from the table. Returns true on success, false if key not found.
bool HashTable::remove(string key) {
    hash<string> hasher;
    size_t hash_val = hasher(key);
    size_t homeIndex = hash_val % currentCapacity;

    // Construct the full probe sequence.
    std::vector<size_t> probeIndices;
    probeIndices.push_back(homeIndex);
    for(size_t offset : offsets) {
        probeIndices.push_back((homeIndex + offset) % currentCapacity);
    }

    for (size_t idx : probeIndices) {
        HashTableBucket& currentBucket = tableData[idx];

        if (currentBucket.type == BucketType::NORMAL) {
            if (currentBucket.key == key) {
                // Key found. Mark the bucket as Empty After Remove (EAR) to preserve the probe chain.
                currentBucket.type = BucketType::EAR;
                currentSize--;
                return true; // Removal successful.
            }
        } else if (currentBucket.type == BucketType::ESS) {
            // ESS terminates the search: the key cannot be further down the chain.
            return false;
        }
        // If EAR, continue probing as the key may be further away.
    }

    return false; // Key was not found after completing the probe sequence.
}

// Checks if a key exists in the hash table.
bool HashTable::contains(const string& key) const {
    optional<int> res = get(key);
    bool answer = res.has_value();
    return answer;
}

// Retrieves the value associated with a key. Returns an optional<int> (nullopt if key not found).
optional<int> HashTable::get(const string& key) const {
    hash<string> hasher_for_str;

    size_t hash_val = hasher_for_str(key);
    size_t homeIndex = hash_val % currentCapacity;

    // Construct the full probe sequence.
    std::vector<size_t> probeIndices;
    probeIndices.push_back(homeIndex);
    for (size_t offset : offsets) {
        probeIndices.push_back((homeIndex + offset) % currentCapacity);
    }

    for (size_t probe_index : probeIndices) {
        const HashTableBucket& bucket_to_check = tableData[probe_index];

        if (bucket_to_check.type == BucketType::NORMAL) {
            if (bucket_to_check.key == key) {
                return bucket_to_check.value; // Key found. Return the value.
            }
        } else if (bucket_to_check.type == BucketType::ESS) {
            return nullopt; // ESS terminates the search. Key is not in the table.
        }
        // If EAR, continue probing.
    }

    return nullopt; // Probed all available slots without finding the key or an ESS.
}

// Overloads the subscript operator (operator[]). Returns a reference to the value.
// Note: If the key is not found, this implementation adheres to the requirement of returning a reference
// to an invalid value within the table (Undefined Behavior - UB).
int& HashTable::operator[](const string& key) {
    std::hash<std::string> hasher;
    size_t hash_val = hasher(key);
    size_t homeIndex = hash_val % currentCapacity;

    // Pointer to the last bucket checked; used for the required UB return if the key isn't found.
    HashTableBucket* lastCheckedBucket = nullptr;

    // Construct the full probe sequence.
    std::vector<size_t> probeIndices;
    probeIndices.push_back(homeIndex);
    for (size_t offset : offsets) {
        probeIndices.push_back((homeIndex + offset) % currentCapacity);
    }

    for (size_t probe_index : probeIndices) {
        lastCheckedBucket = &tableData[probe_index];

        if (lastCheckedBucket->type == BucketType::NORMAL) {
            if (lastCheckedBucket->key == key) {
                return lastCheckedBucket->value; // Key found. Return the reference to its value.
            }
        } else if (lastCheckedBucket->type == BucketType::ESS) {
            // ESS terminates the search. Break to return the UB reference.
            break;
        }
    }

    // Returns a reference to the value of the last bucket checked (where search terminated),
    // resulting in the required undefined behavior if the key was not found.
    return lastCheckedBucket->value;
}

// Returns a vector containing all keys currently stored in the table.
vector<string> HashTable::keys() const {
    vector<string> allKeys;
    for (const auto& bucket : tableData) {
        if (bucket.type == BucketType::NORMAL) {
            allKeys.push_back(bucket.key);
        }
    }
    return allKeys;
}

// Calculates and returns the current load factor (alpha = size / capacity).
double HashTable::alpha() const {
    return static_cast<double>(currentSize) / static_cast<double>(currentCapacity);
}

// Returns the total number of buckets available in the table (capacity).
size_t HashTable::capacity() const {
    return currentCapacity;
}

// Returns the number of elements currently stored in the table (size).
size_t HashTable::size() const {
    return currentSize;
}

// Overloads the stream insertion operator for the entire hash table.
ostream& operator<<(ostream& os, const HashTable& hashTable) {
    for (size_t i = 0; i < hashTable.tableData.size(); ++i) {
        const auto& bucket = hashTable.tableData[i];

        if (bucket.type == BucketType::NORMAL) {
            os << "Bucket " << i << ": " << bucket << endl;
        }
    }
    return os;
}

// Doubles the table capacity and rehashes all existing elements.
void HashTable::resize() {
    size_t oldCapacity = currentCapacity;
    currentCapacity *= 2; // New capacity is double the old.

    vector<HashTableBucket> oldTableData = tableData; // Save current data.

    tableData.clear();
    tableData.resize(currentCapacity); // Reallocate and initialize the new table.

    generateOffsets(); // Generate a new random probe sequence for the new capacity.

    currentSize = 0; // Reset size, it will be recounted during rehash.

    // Rehash and re-insert all elements from the old table into the new one.
    for (auto& bucket : oldTableData) {
        if (bucket.type == BucketType::NORMAL) {

            hash<string> hasher;
            size_t newHashVal = hasher(bucket.key);
            size_t homeIndex = newHashVal % currentCapacity; // New home index.

            // Re-insertion requires finding the first empty (ESS) spot in the new table.

            // Probe the home index first.
            size_t probeIndex = homeIndex;

            if (tableData[probeIndex].type == BucketType::ESS) {
                tableData[probeIndex].load(bucket.key, bucket.value);
                currentSize++;
                continue; // Element rehashed, move to the next.
            }

            // Probe using offsets for the remaining spots until an ESS is found.
            for (size_t offset : offsets) {
                probeIndex = (homeIndex + offset) % currentCapacity;

                if (tableData[probeIndex].type == BucketType::ESS) {
                    tableData[probeIndex].load(bucket.key, bucket.value);
                    currentSize++;
                    break; // Element rehashed, move to the next.
                }
            }
        }
    }
}

// Generates a random permutation of offsets for the probing sequence using Fisher-Yates.
void HashTable::generateOffsets() {
    offsets.clear();

    // Populate offsets with 1, 2, ..., capacity-1.
    for (int i = 1; i < currentCapacity; i++) {
        offsets.push_back(i);
    }

    // Shuffle the offsets using the Fisher-Yates algorithm.
    int n = offsets.size();
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap elements.
        size_t temp = offsets[i];
        offsets[i] = offsets[j];
        offsets[j] = temp;
    }
}