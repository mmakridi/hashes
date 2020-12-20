#include "hashes.hpp"
#ifndef HASH_CUCKOO_HPP
#define HASH_CUCKOO_HPP

template<typename Key, typename Value, typename Hash>
class HashMapCuckoo : public HashMap<Key, Value> {
protected:
    std::vector<std::pair<std::pair<Key, size_t>, Value>> data{};
    std::vector<bool> initialized_data{};
    Hash hash_first{};
    Hash hash_second{};
    size_t rehash_tries{3};

public:
    HashMapCuckoo(size_t n);

    void insert(const Key& key, const Value& value, bool try_rehash);
    const Value& find(const Key& key);
    virtual const Value& operator[](const Key& key) {
        return find(key);
    };
    void change_rehash_tries(size_t n) {rehash_tries = n;};
    const Value erase(const Key& key);
    void rehash();
    size_t size();
    void print();
};

template<typename Key, typename Value, typename Hash>
HashMapCuckoo<Key, Value, Hash>::HashMapCuckoo(size_t n){
    uint64_t M = static_cast<uint64_t>(ceil(log2(n)));
    uint64_t lookup_length = static_cast<uint64_t>(ceil(log2(n)));
    uint64_t m = uint64_t(1) << M;
    this->data.resize(m);
    this->initialized_data.resize(m, false);
    this->hash_first.set_table_size(m, M);
    this->hash_second.set_table_size(m, M);
    this->hash_first.set_hash_parameters();
    this->hash_second.set_hash_parameters();
};

template<typename Key, typename Value, typename Hash>
void HashMapCuckoo<Key, Value, Hash>::rehash(){
    hash_first.set_hash_parameters();
    hash_second.set_hash_parameters();
    auto tmp_data = data;
    auto tmp_init = initialized_data;
    initialized_data.resize(m, false);
    for (std::pair<std::pair<Key, size_t>, Value> &elem : tmp_data) {
        insert(elem.first.first, elem.second, false);
    }
};

template<typename Key, typename Value, typename Hash>
void HashMapCuckoo<Key, Value, Hash>::insert(const Key& key, const Value& value, bool try_rehash = true) {
    size_t index_0 = hash_first(key);
    size_t index_1 = hash_second(key);

    // if fits to its 1st position, then OK
    if (!initialized_data[index_0]) {
        data[index_0] = {{key, index_1}, value};
        initialized_data[index_0] = true;
        return;
    }

    // if not, try its 2nd position and maybe move other elements
    auto to_move_elem{{key, index_1}, value};
    for (int i = 0; i < lookup_length; ++i) {
        auto try_key = to_move_elem.first.second;
        if (!initialized_data[try_key]) {
            data[try_key] = to_move_elem;
            initialized_data[try_key] = true;
            return;
        }
        auto tmp = data[try_key];
        data[try_key] = to_move_elem;
        to_move_elem = tmp;
    }
    
    // if unsuccessful in lookup_length steps, then try to rehash all table
    if (try_rehash) {
        for (size_t i = 0; i < rehash_tries; ++i) {
            try {
                rehash();
            } catch (std::overflow_error &e) {
                continue;
            }
            break;
        }
        index_0 = hash_first(key);
        index_1 = hash_second(key);
        // if fits to its 1st position, then OK
        if (!initialized_data[index_0]) {
            data[index_0] = {{key, index_1}, value};
            initialized_data[index_0] = true;
            return;
        }
        // if fits to its 2nd position, then OK
        if (!initialized_data[index_1]) {
            data[index_1] = {{key, index_0}, value};
            initialized_data[index_1] = true;
            return;
        }
    }

    throw std::overflow_error("Couldn't successfully rehash after" + this->rehash_tries + "tries");
};

template<typename Key, typename Value, typename Hash>
const Value& HashMapCuckoo<Key, Value, Hash>::find(const Key& key) {
    size_t index_0 = hash_first(key), index_1{0};
    for (size_t i = 0; i < data.size(), ++i) {
        if (initialized_data[index_0]) {
            if (data[index_0].first.first == key) {
                return data[index_0].second;
            }
            index_0 = dta[index_0].first.second;
            continue;
        }
        throw std::invalid_argument("No value with such key found");
    }
    throw std::invalid_argument("No value with such key found");
};

template<typename Key, typename Value, typename Hash>
const Value HashMapCuckoo<Key, Value, Hash>::erase(const Key& key) {
    size_t index_0 = hash_first(key), index_1{0};
    for (size_t i = 0; i < data.size(), ++i) {
        if (initialized_data[index_0]) {
            if (data[index_0].first.first == key) {
                initialized_data[index_0] = false;
                return data[index_0].second;
            }
            index_0 = data[index_0].first.second;
            continue;
        }
        throw std::invalid_argument("No value with such key found");
    }
    throw std::invalid_argument("No value with such key found");
};

template<typename Key, typename Value, typename Hash>
size_t HashMapCuckoo<Key, Value, Hash>::size() {
    return this->data.size();
};

template<typename Key, typename Value, typename Hash>
void HashMapCuckoo<Key, Value, Hash>::print()
{
    for (size_t i = 0; i < data.size(); i++)
    {
        if(initialized_data[i] == true)
            std::cout << "table index = " << i << ", (" << data[i].first << " ," << data[i].second << ")" << std::endl;
    }
}

#endif //HASH_CUCKOO_HPP
