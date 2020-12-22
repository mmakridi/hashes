#include "hashes.hpp"
#ifndef HASH_OPEN_ADDRESSING_HPP
#define HASH_OPEN_ADDRESSING_HPP

template<typename Key>
class CustomHashLinear : public CustomHash<Key> {
protected:
    uint64_t c_param{0};
public:
    CustomHashLinear(){};
    void set_hash_parameters();
    size_t get_new_key(const size_t attemp_number) {
        return static_cast<uint64_t>(this->cur_hash + attemp_number * c_param) & (this->m - 1);
    }
};

template<typename Key>
void CustomHashLinear<Key>::set_hash_parameters()
{
    CustomHash<Key>::set_hash_parameters();
    std::uniform_int_distribution<uint64_t> dist{0, static_cast<uint64_t>(uint64_t(1) << this->w) - 1};
    c_param = dist(gen);
    //to get с and m  coprime numbers
    if (!(c_param & 1))
        c_param++;
};


template <typename Key>
class CustomHashQuadratic : public CustomHash<Key> {
protected:
    uint64_t c1_param{0};
    uint64_t c2_param{0};
public:
    CustomHashQuadratic(){};
    void set_hash_parameters();
    size_t get_new_key(const size_t attemp_number) {
        return static_cast<uint64_t>(this->cur_hash +
                                     attemp_number * c1_param +
                                     attemp_number * attemp_number * c2_param)
               & (this->m - 1);
    }
};

template<typename Key>
void CustomHashQuadratic<Key>::set_hash_parameters()
{
    CustomHash<Key>::set_hash_parameters();
    std::uniform_int_distribution<uint64_t> dist{0, static_cast<uint64_t>(uint64_t(1) << this->w) - 1};
    c1_param = dist(gen);
    //to get с and m  coprime numbers
    if (!(c1_param & 1))
        c1_param++;

    c2_param = dist(gen);
    //to get с and m  coprime numbers
    if (!(c2_param & 1))
        c2_param++;
};

template <typename Key>
class CustomHashDouble : public CustomHash<Key> {
protected:
    uint64_t a1_param{0};
    uint64_t b1_param{0};
    uint64_t a2_param{0};
    uint64_t b2_param{0};
    uint64_t cur_h1 = 0;
    uint64_t cur_h2 = 0;
public:
    CustomHashDouble(){};
    void set_hash_parameters();

    size_t operator()(const Key& key) {
        cur_h1 = (static_cast<uint64_t>(key * a1_param + b1_param)) >> (this->w - this->M);
        cur_h2 = (static_cast<uint64_t>(key * a2_param + b2_param)) >> (this->w - this->M);
        if (!(cur_h2 & 1))
            cur_h2++;
        return cur_h1;
    };
    size_t get_new_key(const size_t attemp_number) {
        return static_cast<uint64_t>(cur_h1 +
                                     attemp_number * cur_h2)
               & (this->m - 1);
    }
};

template<typename Key>
void CustomHashDouble<Key>::set_hash_parameters()
{
    std::uniform_int_distribution<uint64_t> dist_a{0,
                                                   static_cast<uint64_t>(uint64_t(1) << this->w) - 1};
    std::uniform_int_distribution<uint64_t> dist_b{0,
                                                   static_cast<uint64_t>(uint64_t(1) << (this->w - this->M)) - 1};
    a1_param = dist_a(gen);
    //to get odd number
    if (!(a1_param & 1))
        a1_param++;
    b1_param = dist_b(gen);

    a2_param = dist_a(gen);
    //to get odd number
    if (!(a2_param & 1))
        a2_param++;
    b2_param = dist_b(gen);
};


template<typename Key, typename Value, typename Hash>
class HashMapOpenAddressing : public HashMap<Key, Value> {
protected:
    std::vector<std::pair<Key, Value> > data{};
    std::vector<bool> initialized_data{};
    Hash hash{};

public:
    HashMapOpenAddressing(size_t n);

    bool insert(const Key& key, const Value& value);
    const Value& find(const Key& key);
    virtual const Value& operator[](const Key& key) {
        return find(key);
    };
    void erase(const Key& key);
    size_t size();
    void print();
};

template<typename Key, typename Value, typename Hash>
HashMapOpenAddressing<Key, Value, Hash>::HashMapOpenAddressing(size_t n){
    uint64_t M = static_cast<uint64_t>(ceil(log2(n)));
    uint64_t m = uint64_t(1) << M;
    this->data.resize(m);
    this->initialized_data.resize(m, false);
    this->hash.set_table_size(m, M);
    this->hash.set_hash_parameters();
};

template<typename Key, typename Value, typename Hash>
bool HashMapOpenAddressing<Key, Value, Hash>::insert(const Key& key, const Value& value)
{
    size_t table_index = hash(key);

    for(size_t attemp_number = 1; attemp_number < data.size(); attemp_number++)
    {   if (initialized_data[table_index] == false)
        {
            data[table_index] = {key, value};
            initialized_data[table_index] = true;
            return true;
        }
        else
        {
            table_index = hash.get_new_key(attemp_number);
        }
    }

    throw std::overflow_error("Hash table overflow");
}

template<typename Key, typename Value, typename Hash>
const Value& HashMapOpenAddressing<Key, Value, Hash>::find(const Key& key)
{
    size_t table_index = hash(key);
    for(size_t attemp_number = 1; attemp_number < data.size(); attemp_number++)
    {   if ((data[table_index].first == key) &&
            (initialized_data[table_index] == true))
        {
            return data[table_index].second;
        }
        else
        {
            table_index = hash.get_new_key(attemp_number);
        }
    }
    throw std::invalid_argument("No value with such key found");
}

template<typename Key, typename Value, typename Hash>
void HashMapOpenAddressing<Key, Value, Hash>::erase(const Key& key)
{
    size_t table_index = hash(key);
    for(size_t attemp_number = 1; attemp_number < data.size(); attemp_number++)
    {   if (data[table_index].first == key && initialized_data[table_index] == true)
        {
            initialized_data[table_index] = false;
            return;
        }
        else
        {
            table_index = hash.get_new_key(attemp_number);
        }
    }
    throw std::invalid_argument("Key cannot be removed because not found");
}

template<typename Key, typename Value, typename Hash>
size_t HashMapOpenAddressing<Key, Value, Hash>::size() {
    return this->data.size();
};

template<typename Key, typename Value, typename Hash>
void HashMapOpenAddressing<Key, Value, Hash>::print()
{
    for (size_t i = 0; i < data.size(); i++)
    {
        if(initialized_data[i] == true)
            std::cout << "table index = " << i << ", (" << data[i].first << " ," << data[i].second << ")" << std::endl;
    }
}

#endif //HASH_OPEN_ADDRESSING_HPP
