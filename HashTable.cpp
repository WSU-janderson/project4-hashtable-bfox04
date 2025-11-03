/**
 * HashTable.cpp
 */

#include "HashTable.h"

using namespace std;

HashTableBucket::HashTableBucket():
    type(BucketType::ESS) {}

HashTableBucket::HashTableBucket(string key, int value):
    key(key), value(value), type(BucketType::NORMAL) {}

void HashTableBucket::load(string key, int value) {
    this->key = key;
    this->value = value;
    this->type = BucketType::NORMAL;
}

bool HashTableBucket::isEmpty() const {
    return type == BucketType::ESS || type == BucketType::EAR;
}

ostream& operator<<(ostream& os, const HashTableBucket& bucket) {
    if (bucket.type == BucketType::NORMAL) {
        os << "<" << bucket.key << ", " << bucket.value << ">";
    }
    return os;
}




HashTable::HashTable(size_t initCapacity) {
    int cap_val = initCapacity;
    currentCapacity = cap_val;
    currentSize = 0;
    tableData.resize(currentCapacity);

    srand(0);
    generateOffsets();
}

void HashTable::generateOffsets() {
    offsets.clear();

    for (int i = 1; i < currentCapacity; i++) {
        offsets.push_back(i);
    }

    // Fisher-Yates
    int n = offsets.size();
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // this is the swap part
        size_t temp = offsets[i];
        offsets[i] = offsets[j];
        offsets[j] = temp;
    }
}

double HashTable::alpha() const {
    return (double)currentSize / (float)currentCapacity;
}

size_t HashTable::capacity() const {
    return currentCapacity;
}

size_t HashTable::size() const {
    return currentSize;
}

optional<int> HashTable::get(const string& key) const {
    hash<string> hasher_for_str;

    size_t hash_val = hasher_for_str(key);
    size_t index = hash_val % currentCapacity;

    const HashTableBucket& the_first_bucket = tableData[index];

    if (the_first_bucket.type == BucketType::NORMAL && the_first_bucket.key == key) {
        return the_first_bucket.value; // found it right away
    }

    if (the_first_bucket.type == BucketType::ESS) {
        return nullopt;
    }

    for (int i = 0; i < (int)offsets.size(); i++) {

        size_t offset_value = offsets[i];
        size_t probe_index = (hash_val + offset_value) % currentCapacity;

        const HashTableBucket& bucket_to_check = tableData[probe_index];

        if (bucket_to_check.type == BucketType::NORMAL) {
            if (bucket_to_check.key == key) {
                return bucket_to_check.value;
            }
        } else if (bucket_to_check.type == BucketType::ESS) {
            return nullopt;
        }
    }

    return nullopt;
}

bool HashTable::contains(const string& key) const {
    optional<int> res = get(key);
    bool answer = res.has_value();
    return answer;
}

bool HashTable::remove(string key) {
    hash<string> hasher;

    size_t hash_value = hasher(key);
    int first_index = hash_value % currentCapacity;

    HashTableBucket* the_bucket = &tableData[first_index];

    if (the_bucket->type == BucketType::NORMAL && the_bucket->key == key) {
        the_bucket->type = BucketType::EAR;
        currentSize = currentSize - 1;
        return true;
    }

    if (the_bucket->type == BucketType::ESS) {
        return false; // nothing to remove here
    }

    for (size_t i = 0; i < offsets.size(); i++) {

        size_t offset_val = offsets[i];
        int next_index = (hash_value + offset_val) % currentCapacity;

        HashTableBucket* check_bucket = &tableData[next_index];

        if (check_bucket->type == BucketType::NORMAL) {
            if (check_bucket->key == key) {
                check_bucket->type = BucketType::EAR;
                currentSize--;
                return true;
            }
        } else if (check_bucket->type == BucketType::ESS) {
            return false;
        }
    }

    return false;
}
