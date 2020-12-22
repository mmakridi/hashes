#include <string>
#include <map>
#include <unordered_map>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#include <hash_map>
#include "hashes.hpp"
#include "hash_map_chained.hpp"
#include "hash_open_addressing.hpp"
#include "hash_map_cuckoo.hpp"
#include "benchmark.hpp"
#include <sstream>
#include "fstream"
int main() {
    size_t max_str = 100000;
    size_t max_int = 100000;

    std::vector<std::string> col_names = {"Chained", "Cucko", "Linear", "Quadratic", "Double",
                                          "std::map", "std::hash_map"};
    std::vector<std::vector<double> >times;
/*
    times.push_back(insert_measure_time<std::string, HashMapChained<std::string,
            int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));
    times.push_back(insert_measure_time<std::string, HashMapCuckoo<std::string,
            int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));
    times.push_back(insert_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashLinear<uint64_t> > > >(max_str));
    times.push_back(insert_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashQuadratic<uint64_t> > > >(max_str));
    times.push_back(insert_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashDouble<uint64_t> > > >(max_str));
    times.push_back(insert_measure_time<std::string, std::map<std::string, int> >(max_str));
    times.push_back(insert_measure_time<std::string, std::hash_map<std::string, int> >(max_str));
    std::string filepath = "C:/Users/psmolnik/Downloads/hashes_dir/hashes/data/hash_insert_real_data.csv";
    for(int i = 0; i < 6; i++)
        std::cout << times[i].size() << std::endl;
    write_to_csv(filepath, times, col_names);
    times.clear();

    times.push_back(find_measure_time<std::string, HashMapChained<std::string,
            int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));
    times.push_back(find_measure_time<std::string, HashMapCuckoo<std::string,
            int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));
    times.push_back(find_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashLinear<uint64_t> > > >(max_str));
    times.push_back(find_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashQuadratic<uint64_t> > > >(max_str));
    times.push_back(find_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashDouble<uint64_t> > > >(max_str));
    times.push_back(find_measure_time<std::string, std::map<std::string, int> >(max_str));
    times.push_back(find_measure_time<std::string, std::hash_map<std::string, int> >(max_str));
    filepath = "C:/Users/psmolnik/Downloads/hashes_dir/hashes/data/hash_find_real_data.csv";
    for(int i = 0; i < 6; i++)
        std::cout << times[i].size() << std::endl;
    write_to_csv(filepath, times, col_names);
    times.clear();

    times.push_back(erase_measure_time<std::string, HashMapChained<std::string,
            int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));

    times.push_back(erase_measure_time<std::string, HashMapCuckoo<std::string,
            int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));

    times.push_back(erase_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashLinear<uint64_t> > > >(max_str));

    times.push_back(erase_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashQuadratic<uint64_t> > > >(max_str));

    times.push_back(erase_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashDouble<uint64_t> > > >(max_str));

    times.push_back(erase_measure_time<std::string, std::map<std::string, int> >(max_str));
    times.push_back(erase_measure_time<std::string, std::hash_map<std::string, int> >(max_str));

    filepath = "C:/Users/psmolnik/Downloads/hashes_dir/hashes/data/hash_erase_real_data.csv";
    for(int i = 0; i < 6; i++)
        std::cout << times[i].size() << std::endl;
    write_to_csv(filepath, times, col_names);

*/

    times.push_back(insert_measure_time<int, HashMapChained<int, int, CustomHash<int> > >(max_int));
    times.push_back(insert_measure_time<int, HashMapCuckoo<int, int, CustomHash<int> > >(max_int));
    times.push_back(insert_measure_time<int, HashMapOpenAddressing<int, int, CustomHashLinear<int> > >(max_int));
    times.push_back(insert_measure_time<int, HashMapOpenAddressing<int, int, CustomHashQuadratic<int> > >(max_int));
    times.push_back(insert_measure_time<int, HashMapOpenAddressing<int, int, CustomHashDouble<int> > >(max_int));
    times.push_back(insert_measure_time<int, std::map<int, int> >(max_int));
    times.push_back(insert_measure_time<int, std::hash_map<int, int> >(max_int));
    std::string filepath = "C:/Users/psmolnik/Downloads/hashes_dir/hashes/data/hash_insert_int.csv";
    write_to_csv(filepath, times, col_names);
    times.clear();

    times.push_back(insert_measure_time<std::string, HashMapChained<std::string,
                    int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));
    times.push_back(insert_measure_time<std::string, HashMapCuckoo<std::string,
                   int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));
    times.push_back(insert_measure_time<std::string, HashMapOpenAddressing<std::string,
                    int, CustomHashStrings<CustomHashLinear<uint64_t> > > >(max_str));
    times.push_back(insert_measure_time<std::string, HashMapOpenAddressing<std::string,
                    int, CustomHashStrings<CustomHashQuadratic<uint64_t> > > >(max_str));
    times.push_back(insert_measure_time<std::string, HashMapOpenAddressing<std::string,
                    int, CustomHashStrings<CustomHashDouble<uint64_t> > > >(max_str));
    times.push_back(insert_measure_time<std::string, std::map<std::string, int> >(max_str));
    times.push_back(insert_measure_time<std::string, std::hash_map<std::string, int> >(max_str));
    filepath = "C:/Users/psmolnik/Downloads/hashes_dir/hashes/data/hash_insert_string.csv";

    write_to_csv(filepath, times, col_names);
    times.clear();

    times.push_back(find_measure_time<int, HashMapChained<int, int, CustomHash<int> > >(max_int));
    times.push_back(find_measure_time<int, HashMapCuckoo<int, int, CustomHash<int> > >(max_int));
    times.push_back(find_measure_time<int, HashMapOpenAddressing<int, int, CustomHashLinear<int> > >(max_int));
    times.push_back(find_measure_time<int, HashMapOpenAddressing<int, int, CustomHashQuadratic<int> > >(max_int));
    times.push_back(find_measure_time<int, HashMapOpenAddressing<int, int, CustomHashDouble<int> > >(max_int));
    times.push_back(find_measure_time<int, std::map<int, int> >(max_int));
    times.push_back(find_measure_time<int, std::hash_map<int, int> >(max_int));
    filepath = "C:/Users/psmolnik/Downloads/hashes_dir/hashes/data/hash_find_int.csv";

    write_to_csv(filepath, times, col_names);
    times.clear();

    times.push_back(find_measure_time<std::string, HashMapChained<std::string,
            int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));
    times.push_back(find_measure_time<std::string, HashMapCuckoo<std::string,
            int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));
    times.push_back(find_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashLinear<uint64_t> > > >(max_str));
    times.push_back(find_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashQuadratic<uint64_t> > > >(max_str));
    times.push_back(find_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashDouble<uint64_t> > > >(max_str));
    times.push_back(find_measure_time<std::string, std::map<std::string, int> >(max_str));
    times.push_back(find_measure_time<std::string, std::hash_map<std::string, int> >(max_str));
    filepath = "C:/Users/psmolnik/Downloads/hashes_dir/hashes/data/hash_find_string.csv";
    write_to_csv(filepath, times, col_names);
    times.clear();

    times.push_back(erase_measure_time<int, HashMapChained<int, int, CustomHash<int> > >(max_int));
    times.push_back(erase_measure_time<int, HashMapCuckoo<int, int, CustomHash<int> > >(max_int));
    times.push_back(erase_measure_time<int, HashMapOpenAddressing<int, int, CustomHashLinear<int> > >(max_int));
    times.push_back(erase_measure_time<int, HashMapOpenAddressing<int, int, CustomHashQuadratic<int> > >(max_int));
    times.push_back(erase_measure_time<int, HashMapOpenAddressing<int, int, CustomHashDouble<int> > >(max_int));
    times.push_back(erase_measure_time<int, std::map<int, int> >(max_int));
    times.push_back(erase_measure_time<int, std::hash_map<int, int> >(max_int));
    filepath = "C:/Users/psmolnik/Downloads/hashes_dir/hashes/data/hash_erase_int.csv";
    for(int i = 0; i < 6; i++)
        std::cout << times[i].size() << std::endl;
    write_to_csv(filepath, times, col_names);
    times.clear();


    times.push_back(erase_measure_time<std::string, HashMapChained<std::string,
            int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));

    times.push_back(erase_measure_time<std::string, HashMapCuckoo<std::string,
            int, CustomHashStrings<CustomHash<uint64_t> > > >(max_str));

    times.push_back(erase_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashLinear<uint64_t> > > >(max_str));

    times.push_back(erase_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashQuadratic<uint64_t> > > >(max_str));

    times.push_back(erase_measure_time<std::string, HashMapOpenAddressing<std::string,
            int, CustomHashStrings<CustomHashDouble<uint64_t> > > >(max_str));

    times.push_back(erase_measure_time<std::string, std::map<std::string, int> >(max_str));
    times.push_back(erase_measure_time<std::string, std::hash_map<std::string, int> >(max_str));

    filepath = "C:/Users/psmolnik/Downloads/hashes_dir/hashes/data/hash_erase_string.csv";
    for(int i = 0; i < 6; i++)
        std::cout << times[i].size() << std::endl;
    write_to_csv(filepath, times, col_names);


    // auto hash_map = HashMapChained<int, std::string>{10};
    // hash_map.insert(1, "first");
    // hash_map.insert(2, "second");
    // std::cout << hash_map.find(1) << std::endl;
    // hash_map.erase(2);
    // hash_map.print();

    //std::cout << 7%3 << std::endl;
    //std::cout << (7&(3-1)) << std::endl;
    //return EXIT_SUCCESS;

//    auto hash_map = HashMapCuckoo<int, std::string, CustomHash<int>>{20};
//    hash_map.insert(1, "first");
//    hash_map.insert(2, "second");
//    std::cout << hash_map.find(2) << std::endl;

    //auto hash_map = HashMapCuckoo<std::string, int, CustomHashStrings<CustomHash<uint64_t>>>{20};
    //hash_map.insert("first", 1);
    //hash_map.insert("second", 2);
    //std::cout << hash_map.find("second");


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
