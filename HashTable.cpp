/**
 * HashTable.cpp
 */

#include "HashTable.h"

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

bool HashTable::insert(std::string key, size_t value) {
    if (alpha() >= 0.5) {
        resize();
    }

    std::hash<std::string> hasher;
    size_t hash_val = hasher(key);
    size_t homeIndex = hash_val % currentCapacity;

    size_t insertionIndex = 0;
    bool foundInsertionSpot = false;

    size_t probeIndex = homeIndex;

    std::vector<size_t> probeIndices;
    probeIndices.push_back(homeIndex);
    for(size_t offset : offsets) {
        probeIndices.push_back((homeIndex + offset) % currentCapacity);
    }

    for(size_t idx : probeIndices) {
        HashTableBucket& currentBucket = tableData[idx];

        if (currentBucket.type == BucketType::NORMAL) {
            if (currentBucket.key == key) {
                return false;
            }
        } else {
            if (!foundInsertionSpot) {
                insertionIndex = idx;
                foundInsertionSpot = true;
            }

            if (currentBucket.type == BucketType::ESS) {

                if (foundInsertionSpot) {
                    tableData[insertionIndex].load(key, value);
                    currentSize++;
                    return true;
                }

                break;
            }

        }
    }


    if (foundInsertionSpot) {
        tableData[insertionIndex].load(key, value);
        currentSize++;
        return true;
    }


    return false;
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

bool HashTable::contains(const string& key) const {
    optional<int> res = get(key);
    bool answer = res.has_value();
    return answer;
}

optional<int> HashTable::get(const string& key) const {
    hash<string> hasher_for_str;

    size_t hash_val = hasher_for_str(key);
    size_t index = hash_val % currentCapacity;

    const HashTableBucket& the_first_bucket = tableData[index];

    if (the_first_bucket.type == BucketType::NORMAL && the_first_bucket.key == key) {
        return the_first_bucket.value;
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

int& HashTable::operator[](const string& key) {
    std::hash<std::string> hasher;
    size_t hash_val = hasher(key);
    size_t homeIndex = hash_val % currentCapacity;

    HashTableBucket* currentBucket = &tableData[homeIndex];

    if (currentBucket->type == BucketType::NORMAL && currentBucket->key == key) {
        return currentBucket->value;
    }

    for (size_t i = 0; i < offsets.size(); ++i) {
        size_t offset_value = offsets[i];
        size_t probe_index = (homeIndex + offset_value) % currentCapacity;

        currentBucket = &tableData[probe_index];

        if (currentBucket->type == BucketType::NORMAL) {
            if (currentBucket->key == key) {

                return currentBucket->value;
            }
        } else if (currentBucket->type == BucketType::ESS) {

            break;
        }

    }

    return currentBucket->value;
}

vector<string> HashTable::keys() const {
    vector<string> allKeys;
    for (const auto& bucket : tableData) {
        if (bucket.type == BucketType::NORMAL) {
            allKeys.push_back(bucket.key);
        }
    }
    return allKeys;
}

double HashTable::alpha() const {
    return static_cast<double>(currentSize) / static_cast<double>(currentCapacity);
}

size_t HashTable::capacity() const {
    return currentCapacity;
}

size_t HashTable::size() const {
    return currentSize;
}

ostream& operator<<(ostream& os, const HashTable& hashTable) {
    for (size_t i = 0; i < hashTable.tableData.size(); ++i) {
        const auto& bucket = hashTable.tableData[i];

        if (bucket.type == BucketType::NORMAL) {
            os << "Bucket " << i << ": " << bucket << endl;
        }
    }
    return os;
}

void HashTable::resize() {
    size_t oldCapacity = currentCapacity;
    currentCapacity *= 2;

    vector<HashTableBucket> oldTableData = tableData;

    tableData.clear();
    tableData.resize(currentCapacity);

    generateOffsets();

    currentSize = 0;

    for (auto& bucket : oldTableData) {
        if (bucket.type == BucketType::NORMAL) {

            hash<string> hasher;
            size_t newHashVal = hasher(bucket.key);
            size_t homeIndex = newHashVal % currentCapacity;

            size_t probeIndex = homeIndex;
            bool rehashSuccess = false;

            if (tableData[probeIndex].type != BucketType::NORMAL) {
                tableData[probeIndex].load(bucket.key, bucket.value);
                currentSize++;
                rehashSuccess = true;
            } else {
                for (size_t i = 0; i < offsets.size(); ++i) {
                    probeIndex = (newHashVal + offsets[i]) % currentCapacity;

                    if (tableData[probeIndex].type != BucketType::NORMAL) {
                        tableData[probeIndex].load(bucket.key, bucket.value);
                        currentSize++;
                        rehashSuccess = true;
                        break;
                    }
                }
            }

        }
    }
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
        size_t temp = offsets[i];
        offsets[i] = offsets[j];
        offsets[j] = temp;
    }
}