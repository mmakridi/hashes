#include <array>
#include <utility>
#include <vector>

#ifndef MUSHROOM_HASH_HASHES_H
#define MUSHROOM_HASH_HASHES_H

template<typename K, typename V>
class HashMap_Chained {
public:
    explicit HashMap_Chained<K, V>(size_t n){
        this->data.resize(n);
    };
    void insert(){};
    size_t size(){
        return this->data.size();
    }

private:
    std::vector<std::pair<K, std::vector<V>>> data;
};

#endif //MUSHROOM_HASH_HASHES_H
