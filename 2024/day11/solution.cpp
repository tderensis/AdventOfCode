#include <print>
#include <fstream>
#include <cstdint>
#include <string>
#include <array>
#include <map>

using Stone      = int64_t;
using StoneCount = uint64_t;
using Stones     = std::map<Stone, StoneCount>;

Stones parse_stones(std::istream& input)
{
    Stones stones;

    Stone    stone  = 0;
    unsigned digits = 0;

    std::string line;
    std::getline(input, line);
    for (char c : line)
    {
        if (c >= '0' && c <= '9')
        {
            stone = stone * 10 + (c - '0');
            ++digits;
        }
        else
        {
            if (digits)
            {
                stones[stone] = 1;
            }
            stone  = 0;
            digits = 0;
        }
    }
    stones[stone] = 1;

    return stones;
}

unsigned digits(auto value)
{
    unsigned d = 1;
    while (value /= 10)
    {
        ++d;
    }
    return d;
}

std::array<Stone, 2> split_stone(Stone s)
{
    auto  s_value         = s;
    auto  s_digits        = digits(s_value);
    Stone split_magnitude = 1;
    auto  split_digits    = s_digits / 2;
    while (split_digits--)
    {
        split_magnitude *= 10;
    }

    auto s1_value = s_value / split_magnitude;

    return {s1_value, s_value - s1_value * split_magnitude};
}

Stones blink(Stones& stones)
{
    Stones new_stones;
    for (const auto& [stone, count] : stones)
    {
        if (stone == 0)
        {
            // If the stone is engraved with the number 0, it is replaced
            // by a stone engraved with the number 1.
            new_stones[1] += count;
        }
        else if (digits(stone) % 2 == 0)
        {
            // If the stone is engraved with a number that has an even
            // number of digits, it is replaced by two stones. The left
            // half of the digits are engraved on the new left stone, and
            // the right half of the digits are engraved on the new right
            // stone. (The new numbers don't keep extra leading zeroes:
            // 1000 would become stones 10 and 0.)
            for (auto s : split_stone(stone))
            {
                new_stones[s] += count;
            }
        }
        else
        {
            // If none of the other rules apply, the stone is replaced by
            // a new stone; the old stone's number multiplied by 2024 is
            // engraved on the new stone.
            new_stones[stone * 2024] += count;
        }
    }

    return new_stones;
}

void print_stones(Stones& stones)
{
    for (auto s : stones)
    {
        std::print("{}:{} ", s.first, s.second);
    }
    std::print("\n");
}

StoneCount count_stones(Stones& stones)
{
    StoneCount count = 0;
    for (auto s : stones)
    {
        count += s.second;
    }
    return count;
}

int main(int argc, char* argv[])
{
    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto stones = parse_stones(inputFile);

    constexpr auto                        num_blinks_p1 = 25;
    constexpr auto                        num_blinks_p2 = 75;
    std::array<StoneCount, num_blinks_p2> stone_count;

    for (size_t i = 0; i < stone_count.size(); ++i)
    {
        stones         = blink(stones);
        stone_count[i] = count_stones(stones);
    }

    std::print("Part 1: {}\n", stone_count[num_blinks_p1 - 1]);
    std::print("Part 2: {}\n", stone_count[num_blinks_p2 - 1]);

    return 0;
}
