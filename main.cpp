#include "hashes.hpp"
#include <string>
#include <map>
#include <unordered_map>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#include <hash_map>
#include "hash_map_chained.hpp"
#include "hash_open_addressing.hpp"

//#include "hash_table.hpp"
//#include "hash_table_list.hpp"

int main() {
    auto hash_map = HashMapChained<int, std::string>{10};
    hash_map.insert(1, "first");
    hash_map.insert(2, "second");
    std::cout << hash_map.find(1) << std::endl;
    hash_map.erase(2);
    hash_map.print();

    // auto fun = CustomHash<int>{};
    // auto res = fun(10);
    // std::cout << res << std::endl;
    // std::cout << (uint64_t) std::string("heh") << std::endl;
/*

    auto hash_map = HashMapOpenAddressing<int, std::string, CustomHashDouble<int> >{10};
    hash_map.insert(1, "first");
    hash_map.insert(2, "second");
    std::cout << hash_map.find(1) << std::endl;
    hash_map.erase(2);
    hash_map.print();
*/

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
