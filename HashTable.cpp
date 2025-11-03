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
