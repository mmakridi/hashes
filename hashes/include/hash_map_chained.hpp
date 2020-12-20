#include "hashes.hpp"
#ifndef HASH_MAP_CHAINED_HPP
#define HASH_MAP_CHAINED_HPP

/*
Hash Map with collisions resolved by chaining
*/

template<typename Key, typename Value>
class HashMapChained : public HashMap<Key, Value> {
private:
    std::vector<std::vector<std::pair<Key, Value> > > data{};
    CustomHash<Key> hash{};

public:
    explicit HashMapChained(size_t n);

    void insert(const Key& key, const Value& value);
    const Value& find(const Key& key);
    virtual const Value& operator[](const Key& key) {
        size_t table_index = this->hash(key);
        for (auto &elem : this->data[table_index]) {
            if (elem.first == key) { return elem.second; }
        }
        throw std::invalid_argument("No value with such key found");
    };

    void erase(const Key& key);

    size_t size();
    void print();
};

template<typename Key, typename Value>
HashMapChained<Key, Value>::HashMapChained(size_t n){
    uint64_t M = static_cast<uint64_t>(ceil(log2(n)));
    uint64_t m = uint64_t(1) << M;
    this->data.resize(m);
    this->hash.set_table_size(m, M);
    this->hash.set_hash_parameters();
};

template<typename Key, typename Value>
void HashMapChained<Key, Value>::insert(const Key& key, const Value& value){
    size_t table_index = this->hash(key);
    for (auto &elem : this->data[table_index]) {
        if (elem.first == key) {
            return;
        }
    }
    this->data[table_index].push_back({key, value});
};

template<typename Key, typename Value>
const Value& HashMapChained<Key, Value>::find(const Key& key) {
    size_t table_index = this->hash(key);
    for (auto &elem : this->data[table_index]) {
        if (elem.first == key) {
            return elem.second;
        }
    }
    throw std::invalid_argument("No value with such key found");
};

template<typename Key, typename Value>
void HashMapChained<Key, Value>::erase(const Key& key) {
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
size_t HashMapChained<Key, Value>::size() {
    return this->data.size();
};

template<typename Key, typename Value>
void HashMapChained<Key, Value>::print() {
    std::cout << "[";
    for (auto& chain : this->data) {
        for (auto& elem : chain) {
            std::cout << "(" << elem.first << ", " << elem.second << "), ";
        }
    }
    std::cout << "]" << std::endl;
};

#endif //HASH_MAP_CHAINED_HPP
