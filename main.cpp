#include <string>
#include <map>
#include <unordered_map>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#include <hash_map>
#include "hashes.hpp"
#include "hash_map_chained.hpp"
#include "hash_open_addressing.hpp"
#include "hash_map_cuckoo.hpp"

int main() {
    // auto hash_map = HashMapChained<int, std::string>{10};
    // hash_map.insert(1, "first");
    // hash_map.insert(2, "second");
    // std::cout << hash_map.find(1) << std::endl;
    // hash_map.erase(2);
    // hash_map.print();
/*
    std::cout << 7%4 << std::endl;
    std::cout << (7&(4-1)) << std::endl;
    return EXIT_SUCCESS;
*/
//    auto hash_map = HashMapCuckoo<int, std::string, CustomHash<int>>{20};
//    hash_map.insert(1, "first");
//    hash_map.insert(2, "second");
//    std::cout << hash_map.find(2) << std::endl;

//    auto hash_map = HashMapCuckoo<std::string, int, CustomHashStrings<CustomHash<uint64_t>>>{20};
//    hash_map.insert("first", 1);
//    hash_map.insert("second", 2);
//    std::cout << hash_map.find("second");


    auto hash_map = HashMapOpenAddressing<std::string, int, CustomHashStrings<CustomHashLinear<uint64_t>>>{10};
    hash_map.insert("first", 1);
    hash_map.insert("second", 2);
    std::cout << hash_map.find("first") << std::endl;
    hash_map.erase("second");
    hash_map.print();

// std::map and std::hash_map experiements
/*
   std::cout << "[std::map\n]";
   std::map<int, char> example1;
   example1.insert({1, 'a'});
   example1.insert({2, 'b'});
   example1.erase(1);
   auto search1 = example1.find(1);
   if (search1 != example1.end()) {
       std::cout << "Found " << search1->first << " " << search1->second << '\n';
   } else {
       std::cout << "Not found\n";
   }
   search1 = example1.find(2);
   if (search1 != example1.end()) {
       std::cout << "Found " << search1->first << " " << search1->second << '\n';
   } else {
       std::cout << "Not found\n";
   }
    std::cout << "[std::hash_map]\n";
    std::hash_map<int, char> example2;
    example2.insert({1, 'a'});
    example2.insert({2, 'b'});
    example2.erase(1);
    auto search2 = example2.find(1);
    if (search2 != example2.end()) {
        std::cout << "Found " << search2->first << " " << search2->second << '\n';
    } else {
        std::cout << "Not found\n";
    }
    search2 = example2.find(2);
    if (search2 != example2.end()) {
        std::cout << "Found " << search2->first << " " << search2->second << '\n';
    } else {
        std::cout << "Not found\n";
    }

*/
    return 0;
}
