#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <cassert>

int item_value(char item)
{
    if (item >= 'a' && item <= 'z')
        return item - 'a' + 1;
    else if (item >= 'A' && item <= 'Z')
        return item - 'A' + 27;
    assert(false && "Invalid rucksack item");
    __builtin_unreachable();
}

int total_value(std::vector<char>& items)
{
    // 'Map' common items into scores, then find sum as required
    int sum = 0;
    for (auto item : items)
        sum += item_value(item);
    return sum;
}


void part1(std::ifstream& file)
{
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

    std::cout << "Part 1 - Total of common items: " << total_value(common_items) << "\n";
}

void part2(std::ifstream& file)
{
    int x = 0; // Tracks the line number in the group of 3
    std::set<char> group_items; // Temp set per group
    std::vector<char> badges; // Stores badge letter
    for (std::string line; std::getline(file, line); x = (x + 1) % 3) {
        if (x == 0) {
            // Load base items
            group_items.clear();
            for (auto a : line)
                group_items.insert(a);
            continue;
        }

        // Look for overlaps - use new set as we would modify set as we go
        std::set<char> new_common;
        for (auto a : group_items) {
            if (line.find(a) != std::string::npos)
                new_common.insert(a);
        }
        group_items = std::move(new_common);

        // Last one, add to the badge collection
        if (x == 2) {
            assert(group_items.size() == 1);
            badges.push_back(*group_items.begin());
        }
    }
    assert(x == 0); // Multiples of 3 implicit assumption

    std::cout << "Part 2 - Total of badges: " << total_value(badges) << "\n";
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    part1(file);

    file.clear();
    file.seekg(0);
    part2(file);

    return 0;
}
