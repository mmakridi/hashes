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
std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max() - 1);
std::uniform_int_distribution<uint64_t> dist_string(1, 9);
#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

std::string get_real_string(std::ifstream& file_real_data)
{
    std::string data;
    std::getline(file_real_data, data);
    return data;
}

std::string get_random_string()
{
    size_t len = dist_string(gen);
    std::string random_string;
    for (std::size_t i = 0; i < len; ++i)
    {
        random_string += characters_set[distribution(gen)];
    }
    return random_string;
}

template<typename Key>
std::vector<std::pair<Key, int> > create_vocabulary(size_t vocab_size)
{
    std::vector<std::pair<Key, int> > vocabulary(vocab_size);
    for(size_t i = 0; i < vocabulary.size(); i++)
    {
        Key key = dist(gen);
        int value = dist(gen);
        vocabulary[i] = {key, value};
    }
    return vocabulary;
}

template<>
std::vector<std::pair<std::string, int> > create_vocabulary<std::string>(size_t vocab_size)
{
    std::vector<std::pair<std::string, int> > vocabulary(vocab_size);
    for(size_t i = 0; i < vocabulary.size(); i++)
    {
        std::string key = get_random_string();
        int value = dist(gen);
        vocabulary[i] = {key, value};
    }
    return vocabulary;
}

std::vector<std::pair<std::string, int> > create_real_data_vocabulary(std::string file_path, size_t vocab_size)
{
    std::ifstream file_real_data(file_path);
    std::vector<std::pair<std::string, int> > vocabulary(vocab_size);
    for(size_t i = 0; i < vocabulary.size(); i++)
    {
        std::string key = get_real_string(file_real_data);
        int value = dist(gen);
        vocabulary[i] = {key, value};
    }
    return vocabulary;
}

template<typename Key, typename HashMapType>
std::vector<double> insert_measure_time(std::vector<std::pair<Key, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = HashMapType{vocabulary.size() * 4};
    for(size_t i = 0; i < vocabulary.size(); i+= 1)
    {
        Key key = vocabulary[i].first;
        int value = vocabulary[i].second;
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert(key, value);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> insert_measure_time<int, std::map<int, int> >(std::vector<std::pair<int, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::map<int, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1)
    {
        int key = vocabulary[i].first;
        int value = vocabulary[i].second;
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert({key, value});
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> insert_measure_time<std::string, std::map<std::string, int> >
        (std::vector<std::pair<std::string, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::map<std::string, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1)
    {
        std::string key = vocabulary[i].first;
        int value = vocabulary[i].second;
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert({key, value});
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> insert_measure_time<int, std::hash_map<int, int> >(std::vector<std::pair<int, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::hash_map<int, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1)
    {
        int key = vocabulary[i].first;
        int value = vocabulary[i].second;
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert({key, value});
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> insert_measure_time<std::string, std::hash_map<std::string, int> >
        (std::vector<std::pair<std::string, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::hash_map<std::string, int> ();
    for(size_t i = 0; i < vocabulary.size(); i+= 1)
    {
        std::string key = vocabulary[i].first;
        int value = vocabulary[i].second;
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert({key, value});
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<typename Key, typename HashMapType>
std::vector<double> find_measure_time(std::vector<std::pair<Key, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = HashMapType{vocabulary.size() * 4};
    for(size_t i = 0; i < vocabulary.size(); i+= 1)
    {
        Key key = vocabulary[i].first;
        int value = vocabulary[i].second;
        hash_map.insert(key, value);
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.find(key);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> find_measure_time<int, std::map<int, int> >
        (std::vector<std::pair<int, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::map<int, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1)
    {
        int key = vocabulary[i].first;
        int value = vocabulary[i].second;
        hash_map.insert({key, value});
        auto t1 = std::chrono::high_resolution_clock::now();
        auto find_result_iterator = hash_map.find(key);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> find_measure_time<std::string, std::map<std::string, int> >
        (std::vector<std::pair<std::string, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::map<std::string, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1)
    {
        std::string key = vocabulary[i].first;
        int value = vocabulary[i].second;
        hash_map.insert({key, value});
        auto t1 = std::chrono::high_resolution_clock::now();
        auto find_result_iterator = hash_map.find(key);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> find_measure_time<int, std::hash_map<int, int> >
        (std::vector<std::pair<int, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::hash_map<int, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1)
    {
        int key = vocabulary[i].first;
        int value = vocabulary[i].second;
        hash_map.insert({key, value});
        auto t1 = std::chrono::high_resolution_clock::now();
        auto find_result_iterator = hash_map.find(key);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> find_measure_time<std::string, std::hash_map<std::string, int> >
        (std::vector<std::pair<std::string, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::hash_map<std::string, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1)
    {
        std::string key = vocabulary[i].first;
        int value = vocabulary[i].second;
        hash_map.insert({key, value});
        auto t1 = std::chrono::high_resolution_clock::now();
        auto find_result_iterator = hash_map.find(key);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<typename Key, typename HashMapType>
std::vector<double> erase_measure_time(std::vector<std::pair<Key, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = HashMapType{vocabulary.size() * 4};
    for(size_t i = 0; i < vocabulary.size(); i+= 1) {
        Key key = vocabulary[i].first;
        int value = vocabulary[i].second;
        hash_map.insert(key, value);
    }
    for(size_t i = 0; i < vocabulary.size(); i+= 1) {
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.erase(vocabulary[i].first);
        auto t2 = std::chrono::high_resolution_clock::now();
        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    std::reverse( results.begin(), results.end() );

    return results;
}

template<>
std::vector<double> erase_measure_time<int, std::map<int, int> >
        (std::vector<std::pair<int, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::map<int, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1) {
        int key = vocabulary[i].first;
        int value = vocabulary[i].second;
        hash_map.insert({key, value});
    }
    for(size_t i = 0; i < vocabulary.size(); i+= 1) {
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.erase(vocabulary[i].first);
        auto t2 = std::chrono::high_resolution_clock::now();
        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    std::reverse( results.begin(), results.end() );

    return results;
}

template<>
std::vector<double> erase_measure_time<std::string, std::map<std::string, int> >
        (std::vector<std::pair<std::string, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::map<std::string, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1) {
        std::string key = vocabulary[i].first;
        int value = vocabulary[i].second;
        hash_map.insert({key, value});
    }
    for(size_t i = 0; i < vocabulary.size(); i+= 1) {
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.erase(vocabulary[i].first);
        auto t2 = std::chrono::high_resolution_clock::now();
        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    std::reverse( results.begin(), results.end() );

    return results;
}

template<>
std::vector<double> erase_measure_time<int, std::hash_map<int, int> >
        (std::vector<std::pair<int, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::hash_map<int, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1) {
        int key = vocabulary[i].first;
        int value = vocabulary[i].second;
        hash_map.insert({key, value});
    }
    for(size_t i = 0; i < vocabulary.size(); i+= 1) {
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.erase(vocabulary[i].first);
        auto t2 = std::chrono::high_resolution_clock::now();
        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    std::reverse( results.begin(), results.end() );

    return results;
}
template<>
std::vector<double> erase_measure_time<std::string, std::hash_map<std::string, int> >
        (std::vector<std::pair<std::string, int> >& vocabulary)
{
    std::vector<double> results;
    auto hash_map = std::hash_map<std::string, int>();
    for(size_t i = 0; i < vocabulary.size(); i+= 1) {
        std::string key = vocabulary[i].first;
        int value = vocabulary[i].second;
        hash_map.insert({key, value});
    }
    for(size_t i = 0; i < vocabulary.size(); i+= 1) {
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.erase(vocabulary[i].first);
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

