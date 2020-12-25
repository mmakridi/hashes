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

template <typename Key, size_t degs = 1>
class CustomHash : public HashFunction<Key> {
protected:
    uint64_t m{1};
    uint64_t M{0};
    uint64_t w{64};
    uint64_t a_param{1};
    std::vector<uint64_t> poly_params{{1}};
    uint64_t degree{1};
    uint64_t b_param{0};
    size_t cur_hash;
public:
    CustomHash(): degree{degs} {};
    void set_table_size(const uint64_t m_, const uint64_t M_) {
        if ((2 << (M_-1)) != (m_)) {throw std::invalid_argument("arguments must satisfy m=2^M");}
        this->m = m_;
        this->M = M_;
    };
    void set_significant_bits(const uint64_t M_) {
        this->M = M_;
        this->m = 2 << (M_ - 1);
    };
    void set_independency_degree(uint64_t n) {
        this->degree = n;
        this->poly_params.resize(n, 1);
    };
    uint64_t table_size() {
        return m;
    };
    void set_hash_parameters() {
        if (M == 0) {throw std::invalid_argument("parameter M was not set, use `set_table_size` prior using this method");}
        std::uniform_int_distribution<uint64_t> dist_a{1, static_cast<uint64_t>(uint64_t(2) << (w-1)) - 1};
        std::uniform_int_distribution<uint64_t> dist_b{0, static_cast<uint64_t>(uint64_t(2) << (w-M-1)) - 1};
        a_param = dist_a(gen);
        //to get odd number
        if (!(a_param & 1))
            a_param++;
        b_param = dist_b(gen);

        std::uniform_int_distribution<uint64_t> dist_c{1, static_cast<uint64_t>(uint64_t(2) << (w-1)) - 1};
        for (size_t i = 0; i < poly_params.size(); ++i) {
            poly_params[i] = dist_c(gen);
        }
    };
    size_t operator()(const Key& key) {
//        cur_hash = 0;
//        uint64_t power{a_param};
//        for (uint64_t deg = 1; deg <= degree; ++deg) {
//            for (size_t j = 2; j <= deg; ++j) {
//                power *= a_param;
//            }
//            cur_hash += key * power;
//            power = a_param;
//        }
//        cur_hash = (cur_hash + b_param) >> (w-M);

        uint64_t tmp_hash{0};
        uint64_t key_degree = static_cast<uint64_t>(key);
        for (uint64_t deg = 1; deg <= degree; ++deg) {
            for (size_t j = 2; j <= deg; ++j) {key_degree *= key;}
            tmp_hash += key_degree * poly_params[deg-1];
            key_degree = static_cast<uint64_t>(key);
        }
        tmp_hash = (tmp_hash + b_param) >> (w-M);
        cur_hash = tmp_hash;

        return cur_hash;
    };

};

template <typename Hash>
class CustomHashStrings {
protected:
//    uint64_t m{2};
//    uint64_t M{1};
    uint64_t w{64};
    uint64_t p{1};
    uint64_t a_param{0};
    size_t curr_hash{0};
    Hash hash{};
public:
    CustomHashStrings(){};
    void set_table_size(const uint64_t m_, const uint64_t M_) {
        hash.set_table_size(m_, M_);
//        this->m = m_;
//        this->M = M_;
    };
    void set_significant_bits(const uint64_t M_) {
        hash.set_significant_bits(M_);
//        this->M = M_;
//        this->m = 2 << (M_ - 1);
    };
    Hash& get_inner_hash_function() {
        return hash;
    };
    void set_independency_degree(uint64_t n) {
        this->hash.set_independency_degree(n);
    };
    void set_prime_modulo() {
        if (hash.table_size() == 1) {throw std::invalid_argument("need to set inner hash table size prior to prime modulo");}
        for (auto elem : primes_asc) {
            if (elem >= hash.table_size()) {
                p = elem;
                break;
            }
        }
    };
    void set_hash_parameters() {
        std::uniform_int_distribution<uint64_t> dist_a{1, p};
        a_param = dist_a(gen);
        hash.set_hash_parameters();
        set_prime_modulo();
    };
    size_t operator()(const std::string& key) {
        size_t length = key.size();
        curr_hash = 0;
        uint64_t power{a_param};
        for (size_t i = 0; i < length; ++i) {
            auto ascii_symbol = static_cast<uint64_t>(key[i]);
            for (size_t j = 2; j <= length-i; ++j) {
                power *= a_param;
            }
            curr_hash += ((ascii_symbol * power) % p);
            curr_hash %= p;
            power = a_param;
        }
        curr_hash = hash(curr_hash);
        return curr_hash;
    };
    size_t get_new_key(const size_t attemp_number) {
//        return static_cast<uint64_t>(this->curr_hash + attemp_number * c_param) & (this->m - 1);
        return hash.get_new_key(attemp_number);
    };
};

#endif //HASHES_HPP