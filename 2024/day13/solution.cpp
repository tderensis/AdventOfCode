#include <print>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include "linalg.h"

struct ClawGame
{
    int64_t x_a;
    int64_t y_a;
    int64_t x_b;
    int64_t y_b;
    int64_t x_p;
    int64_t y_p;
};

using ClawGames = std::vector<ClawGame>;

ClawGames parse_input(std::istream& input)
{
    ClawGames games;

    int line_number = 0;

    ClawGame game;
    for (std::string line; std::getline(input, line);)
    {
        if (line_number % 4 == 0)
        {
            sscanf(line.data(), "Button A: X+%lld, Y+%lld", &game.x_a, &game.y_a);
        }
        else if (line_number % 4 == 1)
        {
            sscanf(line.data(), "Button B: X+%lld, Y+%lld", &game.x_b, &game.y_b);
        }
        else if (line_number % 4 == 2)
        {
            sscanf(line.data(), "Prize: X=%lld, Y=%lld", &game.x_p, &game.y_p);
        }
        else if (line_number % 4 == 3)
        {
            games.push_back(game);
            game = {};
        }
        ++line_number;
    }
    games.push_back(game);

    return games;
}

void print_game(const ClawGame& game)
{
    std::print("Button A: X+{}, Y+{}\n", game.x_a, game.y_a);
    std::print("Button B: X+{}, Y+{}\n", game.x_b, game.y_b);
    std::print("Prize: X={}, Y={}\n", game.x_p, game.y_p);
}

/**
   Solve the claw game using linear algebra and the fact that:
     X * S = Y can be solved S = X^-1 * Y
   Returns the number of tokens taken, or 0 if it could not be solved.
 */
int64_t solve_game(const ClawGame& game)
{
    linalg::aliases::double2x2 X {
        { (double) game.x_a, (double) game.y_a},
        { (double) game.x_b, (double) game.y_b}};
    linalg::aliases::double2x1 Y {{ (double) game.x_p, (double) game.y_p }};

    auto S = mul(inverse(X), Y);

    // Round the presses to the nearest integer and check if we have an
    // exact answer
    auto a_presses = lround(S[0][0]);
    auto b_presses = lround(S[0][1]);

    if (game.x_p == a_presses*game.x_a + b_presses*game.x_b &&
        game.y_p == a_presses*game.y_a + b_presses*game.y_b)
    {
        return a_presses * 3 + b_presses; // A button costs 3 tokens
    }
    return 0;
}

int main(int argc, char* argv[])
{
    std::print("Day 13 Solution\n");

    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto games = parse_input(inputFile);

    int64_t tokens_p1 = 0;
    int64_t tokens_p2 = 0;

    for (auto& game : games)
    {
        tokens_p1 += solve_game(game);
        game.x_p += 10000000000000;
        game.y_p += 10000000000000;
        tokens_p2 += solve_game(game);
    }

    std::print("Total tokens p1 {}\n", tokens_p1);
    std::print("Total tokens p2 {}\n", tokens_p2);
    return 0;
}
