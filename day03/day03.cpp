#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <cassert>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    // Got a bit confused on this one, it's unique for each line but always add into this
    std::vector<char> common_items;

    for (std::string line; std::getline(file, line); ) {
        size_t length = line.length();
        assert(length % 2 == 0);
        size_t midway = length / 2;

        // Load items in first half to set
        std::set<char> items;
        for (size_t i = 0; i < midway; ++i)
            items.insert(line[i]);

        // Then check if they already exist for common item in second half
        std::set<char> unique_items;
        for (size_t i = midway; i < length; ++i) {
            char item = line[i];
            if (items.contains(item))
                unique_items.insert(item);
        }

        for (auto item : unique_items)
            common_items.push_back(item);
    }

    // 'Map' common items into scores, then find sum as required
    int sum = 0;
    for (auto item : common_items) {
        if (item >= 'a' && item <= 'z')
            sum += item - 'a' + 1;
        else if (item >= 'A' && item <= 'Z')
            sum += item - 'A' + 27;
        else
            assert(false && "Invalid rucksack item");
    }

    std::cout << "Total of common items: " << sum << "\n";
    return 0;
}
