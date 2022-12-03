#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    int elf = 0;
    std::vector<int> elf_calories;
    elf_calories.push_back(0);

    for (std::string line; std::getline(file, line); ) {
        if (line.empty()) {
            elf_calories.push_back(0);
            ++elf;
            continue;
        }

        elf_calories[elf] += std::stoi(line);
    }

    std::ranges::sort(elf_calories, std::ranges::greater());
    std::cout << "Top 3 elf calorie counts:\n";
    int sum = 0;
    for (int i = 0; i < 3; ++i) {
        std::cout << " - " << elf_calories[i] << "\n";
        sum += elf_calories[i];
    }
    std::cout << "Total of top 3: " << sum << "\n";

    return 0;
}
