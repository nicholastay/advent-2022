#include <iostream>
#include <fstream>
#include <cassert>
#include <array>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);


    std::string line;
    std::getline(file, line);
    assert(line.size() >= 4);

    constexpr size_t n = 'z' - 'a' + 1; // Enough to store all letters and access quickly
    int processed = -1;

    // Do sliding window
    std::array<bool, n> letters;
    for (size_t i = 3; i < line.size(); ++i) {
        letters.fill(false);

        bool found = true;
        for (size_t a = i - 3; a <= i; ++a) {
            char c = line[a] - 'a';
            if (letters[c]) {
                found = false;
                break;
            }
            letters[c] = true;
        }

        if (found) {
            processed = i + 1; // 1-indexed
            break;
        }
    }

    std::cout << "Part 1 - Characters processed before first marker: " << processed << "\n";
    return 0;
}
