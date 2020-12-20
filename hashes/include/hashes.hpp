#include <array>
#include <utility>
#include <vector>
#include <random>
#include <iostream>
#include <cmath>

#ifndef HASHES_HPP
#define HASHES_HPP

//const uint64_t BigPrime = 2305843009213693951;
//const uint64_t BigPrime = 2305843009213693967;
//const uint64_t BigPrime = 9007199254740991;
//const uint64_t BigPrime = 9007199254740881;
//const uint64_t BigPrime = 67280421310721;  // (2^64+1)/274177
//const uint64_t BigPrime = 2147483647;  // 8th Mersenne number, 2^31-1
//const uint64_t BigPrime = 23456789;
//const uint64_t BigPrime = 2059039;
//const uint64_t BigPrime = 2001511;
//const uint64_t BigPrime = 331999;
//const uint64_t BigPrime = 27361;
//const uint64_t BigPrime = 5039;
const std::vector<uint64_t> primes_asc{
    167,
    239,
    317,
    401,
    719,
    1193,
    1931,
    3119,
    5039,
    7793,
    9377,
    11939,
    18253,
    22189,
    27361,
    331999,
    2001511
};


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
    w = 64 - machine word length for best performance
    a is odd from {0, .. pwr(2, w} - 1}
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

template <typename Hash>
class CustomHashStrings {
protected:
    uint64_t m{2};
    uint64_t M{1};
    uint64_t w{64};
    uint64_t p{0};
    uint64_t a_param{0};
    size_t curr_hash{0};
    Hash hash{};
public:
    CustomHashStrings(){};
    void set_table_size(const uint64_t m, const uint64_t M)
    {
        this->m = m;
        this->M = M;
        for (auto elem : primes_asc) {
            if (elem >= m) {
                p = elem;
                break;
            }
        }
        hash.set_table_size(m, M);
    };
    void set_hash_parameters();
    size_t operator()(const std::string& key) {
        size_t length = key.size();
        curr_hash = 0;
        for (size_t i = 0; i < length; ++i) {
            curr_hash += static_cast<size_t>(
                static_cast<uint64_t>(static_cast<uint64_t>(key[i]) * std::pow(a_param, i)) % p % m
            );
        }
        curr_hash = hash(curr_hash);
        return curr_hash;
    };

};

template <typename Hash>
void CustomHashStrings<Hash>::set_hash_parameters() {
    std::uniform_int_distribution<uint64_t> dist_a{0, p};
    a_param = dist_a(gen);
};

#endif //HASHES_HPP