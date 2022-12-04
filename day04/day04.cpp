#include <iostream>
#include <fstream>
#include <cassert>
#include <charconv>

// Note: pos2 is non-inclusive due to how from_chars works
int parse_int_from_pos(char const* str, std::string::size_type pos1, std::string::size_type pos2)
{
    int n;
    auto result = std::from_chars(str + pos1, str + pos2, n);
    assert(result.ec != std::errc::invalid_argument);
    return n;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    int part1_ranges = 0;
    int part2_ranges = 0;
    for (std::string line; std::getline(file, line); ) {
        std::string::size_type dash1_pos = line.find("-");
        std::string::size_type dash2_pos = line.rfind("-");
        std::string::size_type comma_pos = line.find(",");

        assert(dash1_pos != dash2_pos && line.size() > dash2_pos);

        int e1s1 = parse_int_from_pos(line.data(), 0, dash1_pos);
        int e1s2 = parse_int_from_pos(line.data(), dash1_pos+1, comma_pos);
        int e2s1 = parse_int_from_pos(line.data(), comma_pos+1, dash2_pos);
        int e2s2 = parse_int_from_pos(line.data(), dash2_pos+1, line.size());

        // PART 1
        if (
            (e2s1 >= e1s1 && e2s2 <= e1s2) // e1's stuff is all in e2
         || (e1s1 >= e2s1 && e1s2 <= e2s2) // e2's stuff is all in e1
        ) ++part1_ranges;

        // PART 2
        if (
            (e1s1 <= e2s1 && e1s2 >= e2s1) // at some point, e2 is in e1
         || (e2s1 <= e1s1 && e2s2 >= e1s1) // at some point, e1 is in e2
        ) ++part2_ranges;
    }

    std::cout << "Part 1 - Ranges fully containing the other range: " << part1_ranges << "\n";
    std::cout << "Part 2 - Ranges at least partially containing the other range: " << part2_ranges << "\n";

    return 0;
}
