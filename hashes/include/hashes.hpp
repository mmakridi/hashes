#include <array>
#include <utility>
#include <vector>
#include <random>
#include <iostream>

#ifndef HASHES_HASH_HASHES_H
#define HASHES_HASH_HASHES_H

const uint64_t BigPrime = 2305843009213693951;
std::mt19937 gen{};
std::uniform_int_distribution<size_t> dist{1, BigPrime};

/*
Custom univarsal family of hash functions used for this research

We are casting everything to static_cast<size_t> 
*/

template <typename Key>
class CustomHash {
private:
    uint64_t a_param{0};
    uint64_t b_param{0};
    // with this default table size, this function can be used w/o any hash at all
    size_t table_size{1};

public:
    CustomHash();
    explicit CustomHash(size_t table_size);

    void resize(size_t n);
    size_t operator()(const Key& key);
};

template <typename Key>
CustomHash<Key>::CustomHash(size_t table_size): table_size{table_size} {
    a_param = dist(gen);
    b_param = dist(gen);
};

template <typename Key>
CustomHash<Key>::CustomHash(): table_size{1} {
    a_param = dist(gen);
    b_param = dist(gen);
};

template <typename Key>
void CustomHash<Key>::resize(size_t n) {
    this->table_size = n;
};

// TODO: maybe not `static_cast` but smth else?
template <typename Key>
size_t CustomHash<Key>::operator()(const Key& key) {
    // magic goes here, not 100% sure in it
    return static_cast<size_t>(
        (static_cast<uint64_t>(key) * a_param + b_param) % BigPrime % table_size
    );
};

/*
Hash Map with collisions resolved by chaining
*/

template<typename Key, typename Value>
class HashMap_Chained {
private:
    std::vector<std::vector<std::pair<Key, Value>>> data{};
    CustomHash<Key> hash{};

public:
    explicit HashMap_Chained(size_t n);

    void insert(const Key& key, const Value& value);
    const Value& find(const Key& key);
    void remove(const Key& key);

    size_t size();
    void print();
};

template<typename Key, typename Value>
HashMap_Chained<Key, Value>::HashMap_Chained(size_t n){
    this->data.resize(n);
    this->hash.resize(n);
};

template<typename Key, typename Value>
void HashMap_Chained<Key, Value>::insert(const Key& key, const Value& value){
    size_t table_index = this->hash(key);
    for (auto &elem : this->data[table_index]) {
        if (elem.first == key) {
            return;
        }
    }
    this->data[table_index].push_back({key, value});
};

template<typename Key, typename Value>
const Value& HashMap_Chained<Key, Value>::find(const Key& key) {
    size_t table_index = this->hash(key);
    for (auto &elem : this->data[table_index]) {
        if (elem.first == key) {
            return elem.second;
        }
    }
    throw std::invalid_argument("No value with such key found");
};

template<typename Key, typename Value>
void HashMap_Chained<Key, Value>::remove(const Key& key) {
    size_t table_index = this->hash(key);
    bool found{false};
    size_t del_index{0};
    for (size_t index = 0; index < this->data[table_index].size(); ++index) {
        const auto& elem = this->data[table_index][index];
        if (elem.first == key) {
            found = true;
            del_index = index;
            break;
        }
    }
    if (found) {
        // TODO: does const_iterator support incrementation?
        this->data[table_index].erase(this->data[table_index].begin() + del_index);
    } else {
        throw std::invalid_argument("No value with such key found");
    }
};

template<typename Key, typename Value>
size_t HashMap_Chained<Key, Value>::size() {
    return this->data.size();
};

template<typename Key, typename Value>
void HashMap_Chained<Key, Value>::print() {
    std::cout << "[";
    for (auto& chain : this->data) {
        for (auto& elem : chain) {
            std::cout << "(" << elem.first << ", " << elem.second << "), ";
        }
    }
    std::cout << "]" << std::endl;
};

#endif //HASHES_HASH_HASHES_H
