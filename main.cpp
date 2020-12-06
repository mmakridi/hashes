#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include "hashes.hpp"
//#include "hash_table.hpp"
//#include "hash_table_list.hpp"\

int main() {
//    auto hash_map = HashMap<std::string, int>{20};
//    auto hash_map = std::unordered_map<int, int>{};
//    std::cout << hash_map.size() << std::endl;
//    hash_map.Print();
    auto hash_map = HashMap_Chained<std::string, int>{5};
    // auto a = std::string{"lk"};
    std::cout << hash_map.size() << std::endl;
    return 0;
}
