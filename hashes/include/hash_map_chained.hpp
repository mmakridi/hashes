#include "hashes.hpp"
#ifndef HASH_MAP_CHAINED_HPP
#define HASH_MAP_CHAINED_HPP

/*
Hash Map with collisions resolved by chaining
*/

template<typename Key, typename Value, typename Hash>
class HashMapChained : public HashMap<Key, Value> {
private:
    std::vector<std::vector<std::pair<Key, Value> > > data{};
    Hash hash{};

public:
    explicit HashMapChained(size_t n);

    bool insert(const Key& key, const Value& value);
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

template<typename Key, typename Value, typename Hash>
HashMapChained<Key, Value, Hash>::HashMapChained(size_t n){
    uint64_t M = static_cast<uint64_t>(ceil(log2(n)));
    uint64_t m = uint64_t(1) << M;
    this->data.resize(m);
    this->hash.set_table_size(m, M);
    this->hash.set_hash_parameters();
};

template<typename Key, typename Value, typename Hash>
bool HashMapChained<Key, Value, Hash>::insert(const Key& key, const Value& value){
    size_t table_index = this->hash(key);
    for (auto &elem : this->data[table_index]) {
        if (elem.first == key) {
            return false;
        }
    }
    this->data[table_index].push_back({key, value});
    return true;
};

template<typename Key, typename Value, typename Hash>
const Value& HashMapChained<Key, Value, Hash>::find(const Key& key) {
    size_t table_index = this->hash(key);
    for (auto &elem : this->data[table_index]) {
        if (elem.first == key) {
            return elem.second;
        }
    }
    throw std::invalid_argument("No value with such key found");
};

template<typename Key, typename Value, typename Hash>
void HashMapChained<Key, Value, Hash>::erase(const Key& key) {
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

template<typename Key, typename Value, typename Hash>
size_t HashMapChained<Key, Value, Hash>::size() {
    return this->data.size();
};

template<typename Key, typename Value, typename Hash>
void HashMapChained<Key, Value, Hash>::print() {
    std::cout << "[";
    for (auto& chain : this->data) {
        for (auto& elem : chain) {
            std::cout << "(" << elem.first << ", " << elem.second << "), ";
        }
    }
    std::cout << "]" << std::endl;
};

#endif //HASH_MAP_CHAINED_HPP
