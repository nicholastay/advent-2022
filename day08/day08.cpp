#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

bool visible_tree(std::vector<std::vector<uint8_t>> const& height_map, size_t rt, size_t ct)
{
    // Scan every direction
    bool found;
    auto height = height_map[rt][ct];

    // Up
    found = true;
    assert(rt > 0);
    for (auto r = rt; r-- > 0; ) { // (note: unsigned iterate)
        if (height_map[r][ct] >= height) {
            found = false;
            break;
        }
    }
    if (found) 
        return true;

    // Down
    found = true;
    assert(rt < height_map.size() - 1);
    for (auto r = rt + 1; r < height_map.size(); ++r) {
        if (height_map[r][ct] >= height) {
            found = false;
            break;
        }
    }
    if (found)
        return true;

    // Left
    found = true;
    assert(ct > 0);
    for (auto c = ct; c-- > 0; ) {
        if (height_map[rt][c] >= height) {
            found = false;
            break;
        }
    }
    if (found)
        return true;

    // Right
    found = true;
    assert(ct < height_map.size() - 1);
    for (auto c = ct + 1; c < height_map.size(); ++c) {
        if (height_map[rt][c] >= height) {
            found = false;
            break;
        }
    }
    if (found)
        return true;

    return false;
}

size_t visible_count(std::vector<std::vector<uint8_t>> const& height_map)
{
    size_t n = height_map.size();

    // Find visible. Start with all edges already visible (skip over);
    auto visible = 4 * n - 4;
    for (size_t r = 1; r < n - 1; ++r) {
        for (size_t c = 1; c < n - 1; ++c) {
            if (visible_tree(height_map, r, c))
                ++visible;
        }
    }
    return visible;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    // Looks like it is always a square. Let's peek the size first.
    size_t n;
    {
        auto g = file.tellg();
        std::string line;
        std::getline(file, line);
        n = line.size();
        file.seekg(g, std::ios_base::beg);
    }
    assert(n > 2); // Algo assumes there is a middle

    // Load map of tree into memory
    std::vector<std::vector<uint8_t>> height_map {};
    height_map.reserve(n);
    for (std::string line; std::getline(file, line); ) {
        assert(line.size() == n);
        auto& vec = height_map.emplace_back();
        vec.reserve(n);
        for (auto c : line) {
            assert(c >= '0' && c <= '9');
            vec.push_back(c - '0');
        }
    }
    assert(height_map.size() == n);

    auto visible = visible_count(height_map);
    std::cout << "Part 1 - Visible tree count: " << visible << "\n";
    return 0;
}
