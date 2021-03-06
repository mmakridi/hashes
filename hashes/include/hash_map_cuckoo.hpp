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
    size_t rehash_tries{30};
    uint64_t lookup_length{0};

public:
    explicit HashMapCuckoo(size_t n);

    bool insert(const Key& key, const Value& value, bool try_rehash = true);
    const Value& find(const Key& key);
    virtual const Value& operator[](const Key& key) {
        return find(key);
    };
    void change_rehash_tries(size_t n) {rehash_tries = n;};
    Value erase(const Key& key);
    void rehash();
    size_t size();
    void print();
};

template<typename Key, typename Value, typename Hash>
HashMapCuckoo<Key, Value, Hash>::HashMapCuckoo(size_t n){
    auto M = static_cast<uint64_t>(ceil(log2(n)));
    lookup_length = static_cast<uint64_t>(ceil(log2(n)));
    uint64_t m = uint64_t(1) << M;
    this->data.resize(m);
    this->initialized_data.resize(m, false);

    this->hash_first.set_significant_bits(M);
    this->hash_first.set_hash_parameters();
//    this->hash_first.get_inner_hash_function().set_independency_degree(1);  //TODO: change to more n? logn?

    this->hash_second.set_significant_bits(M);
    this->hash_second.set_hash_parameters();
//    this->hash_second.get_inner_hash_function().set_independency_degree(1);  //TODO: change to more n? logn?
};

template<typename Key, typename Value, typename Hash>
void HashMapCuckoo<Key, Value, Hash>::rehash(){
    hash_first.set_hash_parameters();
    hash_second.set_hash_parameters();
    auto tmp_data = data;
    auto tmp_init = initialized_data;
    initialized_data.resize(data.size(), false);
    try {
//        for (std::pair<std::pair<Key, size_t>, Value> &elem : tmp_data) {
//            insert(elem.first.first, elem.second, false);
//        }
        for (size_t i = 0; i < tmp_data.size(); ++i) {
            if (tmp_init[i]) {
                insert(tmp_data[i].first.first, tmp_data[i].second, false);
            }
        }
    } catch (std::overflow_error &e) {
        data = tmp_data;
        initialized_data = tmp_init;
        throw e;
    }
};

template<typename Key, typename Value, typename Hash>
bool HashMapCuckoo<Key, Value, Hash>::insert(const Key& key, const Value& value, bool try_rehash) {
//    std::cout << "key: " << key << std::endl;
    size_t index_0 = hash_first(key);
    size_t index_1 = hash_second(key);
//    std::cout << index_0 << " " << index_1 << std::endl;

    // if fits to its 1st position, then OK
    if (!initialized_data[index_0]) {
        data[index_0] = {{key, index_1}, value};
        initialized_data[index_0] = true;
        return true;
    } else if (data[index_0].first.first == key) {
//        data[index_0].second = value;
        return false;
    }

//    std::cout << "shifting" << std::endl;
    // if not, try its 2nd position and maybe move other elements
    size_t try_key = index_1;
    std::pair<std::pair<Key, size_t>, Value> to_move_elem{{key, index_0}, value};
    for (int i = 0; i < lookup_length; ++i) {
//        try_key = to_move_elem.first.second;
        if (!initialized_data[try_key]) {
            data[try_key] = to_move_elem;
            initialized_data[try_key] = true;
            return true;
        } else if (data[try_key].first.first == key) {
//            data[try_key].second = value;
            return false;
        }
        std::pair<std::pair<Key, size_t>, Value> tmp_elem{{data[try_key].first.first, try_key}, data[try_key].second};
        auto tmp_key = data[try_key].first.second;
        data[try_key] = to_move_elem;
//        auto tmp_key = tmp_elem.first.second;
//        to_move_elem = data[try_key];
        try_key = tmp_key;
        to_move_elem = tmp_elem;
//        try_key = tmp_elem.first.second;
//        auto tmp = data[try_key];
//        to_move_elem = tmp;
    }
    
    // if unsuccessful in lookup_length steps, then try to rehash all table
    if (try_rehash) {
        // saving to_move_elem to arbitrary place in table not to lose it
        // +1 linear complexity iteration in case rehash happens, doesn't change asymptotic
        for (size_t i = 0; i < data.size(); ++i) {
            if (!initialized_data[i]) {
                data[i] = to_move_elem;
                break;
            }
        }
        for (size_t i = 0; i < rehash_tries; ++i) {
            try {
                //std::cout << "rehashing" << std::endl;
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
            return true;
        } else if (data[index_0].first.first == key) {
//            data[index_0].second = value;
            return false;
        }
        // if fits to its 2nd position, then OK
        if (!initialized_data[index_1]) {
            data[index_1] = {{key, index_0}, value};
            initialized_data[index_1] = true;
            return true;
        } else if (data[index_1].first.first == key) {
//            data[index_1].second = value;
            return false;
        }
    }

    throw std::overflow_error("Couldn't successfully rehash after several tries");
};

template<typename Key, typename Value, typename Hash>
const Value& HashMapCuckoo<Key, Value, Hash>::find(const Key& key) {
    size_t index_0 = hash_first(key);
    size_t index_1 = hash_second(key);

    if (initialized_data[index_0] && data[index_0].first.first == key) {
        return data[index_0].second;
    }

    if (initialized_data[index_1] && data[index_1].first.first == key) {
        return data[index_1].second;
    }

    throw std::invalid_argument("No value with such key found");
};

template<typename Key, typename Value, typename Hash>
Value HashMapCuckoo<Key, Value, Hash>::erase(const Key& key) {
    size_t index_0 = hash_first(key);
    size_t index_1 = hash_second(key);

    if (initialized_data[index_0] && data[index_0].first.first == key) {
        initialized_data[index_0] = false;
        return data[index_0].second;
    }

    if (initialized_data[index_1] && data[index_1].first.first == key) {
        initialized_data[index_1] = false;
        return data[index_1].second;
    }

//    throw std::invalid_argument("No value with such key found");
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
        if(initialized_data[i])
            std::cout << "table index = " << i << ", (" << data[i].first << " ," << data[i].second << ")" << std::endl;
    }
}

#endif //HASH_CUCKOO_HPP
