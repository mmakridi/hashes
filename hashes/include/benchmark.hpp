//
// Created by psmolnik on 12/21/2020.
//
#include "hashes.hpp"
#include "hash_map_chained.hpp"
#include "hash_map_cuckoo.hpp"
#include <chrono>
#include <fstream>
#include "map"

std::uniform_int_distribution<uint64_t> dist(0, std::numeric_limits<uint64_t>::max());

#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

template<typename HashMapType>
std::vector<double> insert_measure_time()
{
    std::vector<double> results;
    size_t max = 20000;
    //size_t step = 10;
    auto hash_map = HashMapType{max * 2};
    for(size_t i = 0; i < max; i+= 1)
    {
        uint64_t key = dist(gen);
        uint64_t value = dist(gen);
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert(key, value);
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> insert_measure_time<std::map<int, int> >()
{
    std::vector<double> results;
    size_t max = 20000;
    //size_t step = 10;
    auto hash_map = std::map<int, int>();
    for(size_t i = 0; i < max; i+= 1)
    {
        uint64_t key = dist(gen);
        uint64_t value = dist(gen);
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert({key, value});
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

template<>
std::vector<double> insert_measure_time<std::hash_map<int, int> >()
{
    std::vector<double> results;
    size_t max = 20000;
    //size_t step = 10;
    auto hash_map = std::map<int, int>();
    for(size_t i = 0; i < max; i+= 1)
    {
        uint64_t key = dist(gen);
        uint64_t value = dist(gen);
        auto t1 = std::chrono::high_resolution_clock::now();
        hash_map.insert({key, value});
        auto t2 = std::chrono::high_resolution_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }
    return results;
}

void write_to_csv(const std::string filename,
                  const std::vector<std::vector<double> >& data,
                  const std::vector<std::string>& col_names)
{
    std::ofstream my_file(filename);
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

