//
// Created by psmolnik on 12/21/2020.
//
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#include "hashes.hpp"
#include "hash_map_chained.hpp"
#include "hash_map_cuckoo.hpp"
#include <chrono>
#include <fstream>
#include "map"
#include "hash_map"
const std::string characters_set = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
std::uniform_int_distribution<> distribution(0, characters_set.size() - 1);
std::uniform_int_distribution<uint64_t> dist(0, std::numeric_limits<uint64_t>::max());
std::uniform_int_distribution<uint64_t> dist_string(1, 9);

bool test_real_data = false;
std::ifstream file_real_data("C:\\Users\\psmolnik\\Downloads\\hashes_dir\\hashes\\data\\words_alpha.txt");
#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

template<typename Key>
Key real_string()
{
    Key data;
    //std::getline(file_real_data, data);
    return data;
}

template<>
std::string real_string<std::string>()
{
    std::string data;
    std::getline(file_real_data, data);
    return data;
}

template<typename T>
T random_key()
{
    uint64_t key = dist(gen);
    return key;
}

template<>
std::string random_key<std::string>()
{
    size_t len = dist_string(gen);
    std::string random_string;
    for (std::size_t i = 0; i < len; ++i)
    {
        random_string += characters_set[distribution(gen)];
    }
    return random_string;
}

template<typename Key, typename HashMapType>
std::vector<double> insert_measure_time(size_t max_iter)
{
    std::vector<double> results;
    auto hash_map = HashMapType{max_iter * 4};
    for(size_t i = 0; i < max_iter; i+= 1)
    {
        //std::cout << i << std::endl;
        Key key;
        if(test_real_data) key = real_string<Key>();
        else key = random_key<Key>();

        uint64_t value = random_key<int>();
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert(key, value);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> insert_measure_time<int, std::map<int, int> >(size_t max_iter)
{
    std::vector<double> results;
    //size_t step = 10;
    auto hash_map = std::map<int, int>();
    for(size_t i = 0; i < max_iter; i+= 1)
    {
        uint64_t key = random_key<int>();
        uint64_t value = random_key<int>();
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert({key, value});
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> insert_measure_time<std::string, std::map<std::string, int> >(size_t max_iter)
{
    std::vector<double> results;
    auto hash_map = std::map<std::string, int>();
    for(size_t i = 0; i < max_iter; i+= 1)
    {
        std::string key;
        if(test_real_data) key = real_string<std::string>();
        else key = random_key<std::string>();
        uint64_t value = random_key<int>();
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert({key, value});
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> insert_measure_time<int, std::hash_map<int, int> >(size_t max_iter)
{
    std::vector<double> results;
    auto hash_map = std::hash_map<int, int>();
    for(size_t i = 0; i < max_iter; i+= 1)
    {
        uint64_t key = random_key<int>();
        uint64_t value = random_key<int>();
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert({key, value});
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> insert_measure_time<std::string, std::hash_map<std::string, int> >(size_t max_iter)
{
    std::vector<double> results;
    auto hash_map = std::hash_map<std::string, int> ();
    for(size_t i = 0; i < max_iter; i+= 1)
    {
        std::string key;
        if(test_real_data) key = real_string<std::string>();
        else key = random_key<std::string>();
        uint64_t value = random_key<int>();
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert({key, value});
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<typename Key, typename HashMapType>
std::vector<double> find_measure_time(size_t max_iter)
{
    std::vector<double> results;
    auto hash_map = HashMapType{max_iter * 4};
    for(size_t i = 0; i < max_iter; i+= 1)
    {
        Key key;
        if(test_real_data) key = real_string<Key>();
        else key = random_key<Key>();
        uint64_t value = random_key<int>();
        hash_map.insert(key, value);
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.find(key);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> find_measure_time<int, std::map<int, int> >(size_t max_iter)
{
    std::vector<double> results;
    auto hash_map = std::map<int, int>();
    for(size_t i = 0; i < max_iter; i+= 1)
    {
        uint64_t key = random_key<int>();
        uint64_t value = random_key<int>();
        hash_map.insert({key, value});
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.find(key);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> find_measure_time<std::string, std::map<std::string, int> >(size_t max_iter)
{
    std::vector<double> results;
    auto hash_map = std::map<std::string, int>();
    for(size_t i = 0; i < max_iter; i+= 1)
    {
        std::string key;
        if(test_real_data) key = real_string<std::string>();
        else key = random_key<std::string>();
        uint64_t value = random_key<int>();
        hash_map.insert({key, value});
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.find(key);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> find_measure_time<int, std::hash_map<int, int> >(size_t max_iter)
{
    std::vector<double> results;
    auto hash_map = std::hash_map<int, int>();
    for(size_t i = 0; i < max_iter; i+= 1)
    {
        uint64_t key = random_key<int>();
        uint64_t value = random_key<int>();
        hash_map.insert({key, value});
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.find(key);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> find_measure_time<std::string, std::hash_map<std::string, int> >(size_t max_iter)
{
    std::vector<double> results;
    auto hash_map = std::hash_map<std::string, int>();
    for(size_t i = 0; i < max_iter; i+= 1)
    {
        std::string key;
        if(test_real_data) key = real_string<std::string>();
        else key = random_key<std::string>();
        uint64_t value = random_key<int>();
        hash_map.insert({key, value});
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.find(key);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<typename Key, typename HashMapType>
std::vector<double> erase_measure_time(size_t max_iter)
{
    std::vector<double> results;
    std::vector<Key> keys;
    auto hash_map = HashMapType{max_iter * 4};
    while(keys.size() != max_iter) {
        Key key;
        if(test_real_data) key = real_string<Key>();
        else key = random_key<Key>();
        uint64_t value = random_key<int>();
        bool done = hash_map.insert(key, value);
        if (done)
            keys.push_back(key);
    }
    for(size_t i = 0; i < keys.size(); i+= 1) {
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.erase(keys[i]);
        auto t2 = std::chrono::high_resolution_clock::now();
        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    std::reverse( results.begin(), results.end() );

    return results;
}

template<>
std::vector<double> erase_measure_time<int, std::map<int, int> >(size_t max_iter)
{
    std::vector<double> results;
    std::vector<int> keys;
    auto hash_map = std::map<int, int>();
    for(size_t i = 0; i < max_iter; i+= 1) {
        int key = random_key<int>();
        int value = random_key<int>();
        hash_map.insert({key, value});
        keys.push_back(key);
    }
    for(size_t i = 0; i < keys.size(); i+= 1) {
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.erase(keys[i]);
        auto t2 = std::chrono::high_resolution_clock::now();
        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    std::reverse( results.begin(), results.end() );

    return results;
}

template<>
std::vector<double> erase_measure_time<std::string, std::map<std::string, int> >(size_t max_iter)
{
    std::vector<double> results;
    std::vector<std::string> keys;
    auto hash_map = std::map<std::string, int>();
    for(size_t i = 0; i < max_iter; i+= 1) {
        std::string key;
        if(test_real_data) key = real_string<std::string>();
        else key = random_key<std::string>();
        int value = random_key<int>();
        hash_map.insert({key, value});
        keys.push_back(key);
    }
    for(size_t i = 0; i < keys.size(); i+= 1) {
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.erase(keys[i]);
        auto t2 = std::chrono::high_resolution_clock::now();
        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    std::reverse( results.begin(), results.end() );

    return results;
}

template<>
std::vector<double> erase_measure_time<int, std::hash_map<int, int> >(size_t max_iter)
{
    std::vector<double> results;
    std::vector<int> keys;
    auto hash_map = std::hash_map<int, int>();
    for(size_t i = 0; i < max_iter; i+= 1) {
        int key = random_key<int>();
        int value = random_key<int>();
        hash_map.insert({key, value});
        keys.push_back(key);
    }
    for(size_t i = 0; i < keys.size(); i+= 1) {
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.erase(keys[i]);
        auto t2 = std::chrono::high_resolution_clock::now();
        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    std::reverse( results.begin(), results.end() );

    return results;
}
template<>
std::vector<double> erase_measure_time<std::string, std::hash_map<std::string, int> >(size_t max_iter)
{
    std::vector<double> results;
    std::vector<std::string> keys;
    auto hash_map = std::hash_map<std::string, int>();
    for(size_t i = 0; i < max_iter; i+= 1) {
        std::string key;
        if(test_real_data) key = real_string<std::string>();
        else key = random_key<std::string>();
        int value = random_key<int>();
        hash_map.insert({key, value});
        keys.push_back(key);
    }
    for(size_t i = 0; i < keys.size(); i+= 1) {
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.erase(keys[i]);
        auto t2 = std::chrono::high_resolution_clock::now();
        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    std::reverse( results.begin(), results.end() );

    return results;
}

void write_to_csv(const std::string filename,
                  const std::vector<std::vector<double> >& data,
                  const std::vector<std::string>& col_names)
{
    std::ofstream my_file(filename, std::ofstream::trunc);
    std::cout << my_file.is_open() << std::endl;
    for(int i = 0; i < data.size(); ++i)
    {
        my_file << col_names[i] << ",";
        for(int j = 0; j <  data[i].size(); ++j)
        {
            my_file << data[i][j];
            if(j != data[i].size() - 1)
                my_file << ","; // No comma at end of line
        }
        my_file << "\n";
    }
    my_file.close();
}
#endif //BENCHMARK_HPP

