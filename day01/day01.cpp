#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    int elf = 1;
    int elf_calories = 0;
    int best_elf = 0;
    int best_elf_calories = 0;
    for (std::string line; std::getline(file, line); ) {
        if (line.empty()) {
            if (elf_calories > best_elf_calories) {
                best_elf = elf;
                best_elf_calories = elf_calories;
            }

            elf_calories = 0;
            ++elf;
            continue;
        }

        elf_calories += std::stoi(line);
    }

    if (elf_calories > best_elf_calories)
        best_elf = elf;

    std::cout << "Task 1 - Best elf: " << best_elf << " at " << best_elf_calories << " calories.\n";
    return 0;
}
