#include <print>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using Level = std::vector<int>;

std::vector<Level> parse_input(std::istream& input)
{
    std::vector<Level> levels;
    std::string        line;

    while (std::getline(input, line))
    {
        std::stringstream stream(line);

        Level level;
        while (!stream.eof())
        {
            int         number;
            std::string s;
            stream >> s;
            if (std::stringstream(s) >> number)
            {
                level.push_back(number);
            }
        }

        levels.push_back(level);
    }

    return levels;
}

bool levelIsSafe(const Level& level)
{
    int first      = level[0];
    int second     = level[1];
    int difference = abs(first - second);

    if (difference < 1 || difference > 3)
    {
        return false;
    }

    bool previousAscending = second > first;
    int  previous          = second;
    for (std::size_t i = 2; i < level.size(); ++i)
    {
        int  current          = level[i];
        bool currentAscending = current > previous;

        if (previousAscending != currentAscending)
        {
            return false;
        }
        int difference = abs(current - previous);
        if (difference < 1 || difference > 3)
        {
            return false;
        }
        previousAscending = currentAscending;
        previous          = current;
    }
    return true;
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

    auto levels = parse_input(inputFile);

    int numSafe           = 0;
    int numSafeOneRemoved = 0;

    for (const auto& level : levels)
    {
        if (levelIsSafe(level))
        {
            ++numSafe;
            ++numSafeOneRemoved;
        }
        else
        {
            for (std::size_t i = 0; i < level.size(); ++i)
            {
                Level levelOneRemoved = level;
                levelOneRemoved.erase(levelOneRemoved.begin() + i);
                if (levelIsSafe(levelOneRemoved))
                {
                    ++numSafeOneRemoved;
                    break;
                }
            }
        }
    }

    std::print("Part 1: {}\n", numSafe);
    std::print("Part 2: {}\n", numSafeOneRemoved);

    return 0;
}
