#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include "hashes.hpp"
//#include "hash_table.hpp"
//#include "hash_table_list.hpp"

int main() {
    auto hash_map = HashMap_Chained<int, std::string>{100000};
    hash_map.insert(1, "first");
    hash_map.insert(2, "second");
    std::cout << hash_map.find(1) << std::endl;
    hash_map.remove(2);
    hash_map.print();

    // auto fun = CustomHash<int>{};
    // auto res = fun(10);
    // std::cout << res << std::endl;
    // std::cout << (uint64_t) std::string("heh") << std::endl;

    return 0;
}
