#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <sstream>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    // Read first line for info
    size_t crate_line_size;
    {
        std::string line;
        std::getline(file, line);
        crate_line_size = line.size();
        file.seekg(0);
    }

    // "[X] " per thing except last one
    assert((crate_line_size + 1) % 4 == 0);
    size_t crates = (crate_line_size + 1) / 4;

    // Setup 2D vector to store these things in
    std::vector<std::vector<char>> stacks;
    stacks.reserve(crates);
    for (size_t i = 0; i < crates; ++i)
        stacks.emplace_back();

    bool loading = true; // Loading pre-existing arrangement
    for (std::string line; std::getline(file, line); ) {
        if (loading) {
            if (line.starts_with(" 1 ")) {
                loading = false;

                // Reverse every stack so it's a... stack
                for (auto& stack : stacks)
                    std::reverse(stack.begin(), stack.end());

                continue;
            }

            // Iterate whole line, it will be loading into different crate
            assert(line.size() == crate_line_size);
            for (size_t crate = 0; crate < crates; ++crate) {
                size_t idx = crate * 4;
                if (line[idx] == '[' && line[idx+2] == ']')
                    stacks[crate].push_back(line[idx+1]);
            }

            continue;
        }

        // Ignore empty lines
        if (line.empty())
            continue;

        // Process instructions - go word by word
        std::istringstream iss(line);
        std::string word;
        int times;
        size_t from, to;
        for (size_t i = 0; i < 6; ++i) {
            if (!std::getline(iss, word, ' '))
                assert(false);
            switch (i) {
            case 0:
                assert(word == "move"); break;
            case 1:
                times = std::atoi(word.c_str()); break;
            case 2:
                assert(word == "from"); break;
            case 3:
                from = std::atoi(word.c_str()) - 1; break;
            case 4:
                assert(word == "to"); break;
            case 5:
                to = std::atoi(word.c_str()) - 1; break;
            }
        }
        assert(from < crates && to < crates);
        
        // Do the move
        auto& stack_from = stacks.at(from);
        auto& stack_to = stacks.at(to);
        for (int i = 0; i < times; ++i) {
            assert(stack_from.size() >= 1);
            char item = stack_from.back();
            stack_from.pop_back();
            stack_to.push_back(item);
        }
    }

    std::cout << "Stack layout, from 1 to N:\n";
    for (auto const& stack : stacks) {
        std::cout << " ";
        for (auto c : stack) {
            std::cout << c << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    std::cout << "Part 1 - Top of each stack: ";
    for (auto const& stack : stacks) {
        std::cout << stack.back();
    }
    std::cout << "\n";
}
