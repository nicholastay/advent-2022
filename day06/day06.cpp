#include <iostream>
#include <fstream>
#include <cassert>
#include <array>

size_t find_marker(std::string const& line, size_t window_size)
{
    assert(line.size() >= window_size);

    // Do sliding window
    constexpr size_t n = 'z' - 'a' + 1; // Enough to store all letters and access quickly
    std::array<bool, n> letters;

    for (size_t i = window_size - 1; i < line.size(); ++i) {
        letters.fill(false);

        bool found = true;
        for (size_t a = i - window_size + 1; a <= i; ++a) {
            char c = line[a] - 'a';
            if (letters[c]) {
                found = false;
                break;
            }
            letters[c] = true;
        }

        if (found)
            return i + 1; // 1-indexed
    }

    assert(false && "unreachable");
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    std::string line;
    std::getline(file, line);

    size_t part1 = find_marker(line, 4);
    std::cout << "Part 1 - Characters processed before first marker: " << part1 << "\n";

    size_t part2 = find_marker(line, 14);
    std::cout << "Part 2 - Characters processed before first marker: " << part2 << "\n";
    return 0;
}
