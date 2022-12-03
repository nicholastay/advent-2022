#include <iostream>
#include <fstream>
#include <cassert>
#include <optional>

enum class RPS : int {
    Rock = 1,
    Paper = 2,
    Scissors = 3,
};

enum class Outcome : int {
    Loss = 0,
    Draw = 3,
    Win = 6,
};

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

std::optional<RPS> parse_me(char x)
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

// Assume 'x' is the player, and the outcome is from the POV of that player
Outcome game_outcome(RPS x, RPS y)
{
    if (x == y)
        return Outcome::Draw;

    if (x == RPS::Rock) {
        if (y == RPS::Paper)
            return Outcome::Loss;
        else // y = Scissors
            return Outcome::Win;
    } else if (x == RPS::Paper) {
        if (y == RPS::Rock)
            return Outcome::Win;
        else // y = Scissors
            return Outcome::Loss;
    } else { // x = Scissors
        if (y == RPS::Paper)
            return Outcome::Win;
        else // y = Rock
            return Outcome::Loss;
    }
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    int score = 0;
    for (std::string line; std::getline(file, line); ) {
        assert(line.length() == 3);
        assert(line[1] == ' ');

        auto opponent = parse_opponent(line[0]).value();
        auto me = parse_me(line[2]).value();
        
        auto outcome = game_outcome(me, opponent);
        score += static_cast<int>(me) + static_cast<int>(outcome);
    }

    std::cout << "Final score: " << score << "\n";
    return 0;
}
