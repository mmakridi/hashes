#include <array>
#include <utility>
#include <vector>
#include <random>
#include <iostream>

#ifndef HASHES_HPP
#define HASHES_HPP

std::mt19937 gen{};

// Abstract base class for hash functions
template<typename Key>
class HashFunction {
public:
    HashFunction() {};
    virtual ~HashFunction() = 0;

    virtual size_t operator()(const Key& key) = 0;
};
template<typename Key>
HashFunction<Key>::~HashFunction() {};

// Abstract base class for hash-based data structures (maps) 
template<typename Key, typename Value>
class HashMap {
public:
    HashMap() {};
    virtual ~HashMap() = 0;

    virtual const Value& operator[](const Key& key) = 0;
};
template<typename Key, typename Value>
HashMap<Key, Value>::~HashMap() {};

/*
Custom univarsal family of hash functions used for this research

We are casting everything to static_cast<size_t> 
*/

/*
    Hash rule:
    m is pwr(2, M), table size
    w = 64
    a is from {0, .. pwr(2, w} - 1}, odd number
    b is from {0, .. pwr(2, w - M} - 1}
    h(a, b) = (uint64_t)(ax + b) >> (w - M)
 */

template <typename Key>
class CustomHash : public HashFunction<Key> {
protected:
    uint64_t m{2};
    uint64_t M{1};
    uint64_t w{64};
    uint64_t a_param{0};
    uint64_t b_param{0};
    Key cur_hash;
public:
    CustomHash(){};
    void set_table_size(const uint64_t m, const uint64_t M)
    {
        this->m = m;
        this->M = M;
    };
    void set_hash_parameters();
    size_t operator()(const Key& key) {
        cur_hash = static_cast<size_t>(
                   (static_cast<uint64_t>(key * a_param + b_param)) >> (w - M)
                   );
        return cur_hash;
    };

};

template<typename Key>
void CustomHash<Key>::set_hash_parameters()
{
    std::uniform_int_distribution<uint64_t> dist_a{0, static_cast<uint64_t>(uint64_t(1) << w) - 1};
    std::uniform_int_distribution<uint64_t> dist_b{0, static_cast<uint64_t>(uint64_t(1) << (w - M)) - 1};
    a_param = dist_a(gen);
    //to get odd number
    if (!(a_param & 1))
        a_param++;
    b_param = dist_b(gen);
};

// class CustomHash_Strings {
// private:
//     uint64_t a_param{0};
//     // with this default table size, this function can be used w/o any hash at all
//     size_t table_size{1};
// public:
//     CustomHash_Strings();
//     explicit CustomHash_Strings(size_t table_size);

//     void resize(size_t n);
//     size_t operator()(const std::string& key);
// };

// CustomHash_Strings::CustomHash_Strings(size_t table_size): table_size{table_size} {
//     a_param = dist(gen);
// };

// void CustomHash_Strings::resize(size_t n) {
//     this->table_size = n;
// };

// size_t CustomHash_Strings::operator()(const std::string& key) {
//     return static_cast<size_t>(
//         (static_cast<uint64_t>(key) * a_param) % BigPrime % table_size
//     );
// };

#endif //HASHES_HPP