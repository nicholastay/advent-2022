#include <iostream>
#include <fstream>
#include <cassert>
#include <optional>

enum class RPS : int {
    Rock = 0,
    Paper = 1,
    Scissors = 2,
};

enum class Outcome : int {
    Loss = 0,
    Draw = 3,
    Win = 6,
};

RPS winning_play(RPS r) {
    // This is simply the next value along
    return static_cast<RPS>((static_cast<int>(r) + 1) % 3);
}

RPS losing_play(RPS r) {
    // This is simply the previous value
    // We want Euclidean mod, so we have to -1+3 = +2
    return static_cast<RPS>((static_cast<int>(r) + 2) % 3);
}

std::optional<RPS> parse_opponent(char x)
{
    switch (x) {
    case 'A':
        return RPS::Rock;
    case 'B':
        return RPS::Paper;
    case 'C':
        return RPS::Scissors;
    default:
        return {};
    }
}

std::optional<RPS> part1_parse_me(char x)
{
    switch (x) {
    case 'X':
        return RPS::Rock;
    case 'Y':
        return RPS::Paper;
    case 'Z':
        return RPS::Scissors;
    default:
        return {};
    }
}

std::optional<RPS> part2_parse_and_resolve_me(char x, RPS opponent)
{
    switch (x) {
    case 'X':
        // Need to lose
        return losing_play(opponent);
    case 'Y':
        // Need to draw
        return opponent;
    case 'Z':
        // Need to win
        return winning_play(opponent);
    default:
        return {};
    }
}

// Assume 'x' is the player, and the outcome is from the POV of that player
Outcome game_outcome(RPS x, RPS y)
{
    if (x == y)
        return Outcome::Draw;

    if (winning_play(y) == x)
        return Outcome::Win;

    return Outcome::Loss;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    int part1_score = 0;
    int part2_score = 0;

    for (std::string line; std::getline(file, line); ) {
        assert(line.length() == 3);
        assert(line[1] == ' ');

        auto opponent = parse_opponent(line[0]).value();

        // Part 1 interpretation
        auto part1_me = part1_parse_me(line[2]).value();
        auto part1_outcome = game_outcome(part1_me, opponent);
        part1_score += static_cast<int>(part1_me) + 1 + static_cast<int>(part1_outcome);

        // Part 2 interpretation
        auto part2_me = part2_parse_and_resolve_me(line[2], opponent).value();
        auto part2_outcome = game_outcome(part2_me, opponent);
        part2_score += static_cast<int>(part2_me) + 1 + static_cast<int>(part2_outcome);
    }

    std::cout << "Part 1 score: " << part1_score << "\n";
    std::cout << "Part 2 score: " << part2_score << "\n";
    return 0;
}
