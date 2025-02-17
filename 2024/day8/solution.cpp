#include <print>
#include <fstream>
#include <string>
#include <vector>
#include <map>

struct Position
{
    size_t x;
    size_t y;
};

std::tuple<std::map<char, std::vector<Position>>, size_t, size_t> parse_input(
    std::ifstream& inputFile)
{
    std::map<char, std::vector<Position>> positions;

    size_t y = 0;
    size_t x = 0;
    for (std::string line; std::getline(inputFile, line);)
    {
        x = 0;
        for (char c : line)
        {
            if (c != '.')
            {
                positions[c].push_back({x, y});
            }
            ++x;
        }
        ++y;
    }

    return {positions, x, y};
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

    auto [positions, x, y] = parse_input(inputFile);

    // Create an empty grid of nodes
    std::vector<std::vector<int>> nodes_part1(y, std::vector<int>(x, 0));
    std::vector<std::vector<int>> nodes_part2(y, std::vector<int>(x, 0));

    for (size_t i = 0; i < positions.size(); ++i)
    {
        if (positions[i].size() <= 1)
        {
            continue;
        }

        for (size_t j = 0; j < positions[i].size(); ++j)
        {
            for (size_t k = 0; k < positions[i].size(); ++k)
            {
                if (j == k)
                {
                    continue;
                }

                size_t x1 = positions[i][j].x;
                size_t x2 = positions[i][k].x;
                size_t y1 = positions[i][j].y;
                size_t y2 = positions[i][k].y;

                // Count the antennas themselves as nodes for part 2
                nodes_part2[y1][x1] = 1;
                nodes_part2[y2][x2] = 1;

                size_t xn = x1 - x2 + x1;
                size_t yn = y1 - y2 + y1;

                // Count the nodes for part 1 (and 2)
                if (xn < nodes_part1[0].size() && yn < nodes_part1.size())
                {
                    nodes_part1[yn][xn] = 1;
                    nodes_part2[yn][xn] = 1;

                    x2 = x1;
                    y2 = y1;
                    x1 = xn;
                    y1 = yn;
                    xn = x1 - x2 + x1;
                    yn = y1 - y2 + y1;
                }

                // Count the nodes for part 2
                while (xn < nodes_part2[0].size() && yn < nodes_part2.size())
                {
                    nodes_part2[yn][xn] = 1;

                    x2 = x1;
                    y2 = y1;
                    x1 = xn;
                    y1 = yn;
                    xn = x1 - x2 + x1;
                    yn = y1 - y2 + y1;
                }
            }
        }
    }

    size_t node_count_part1 = 0;
    size_t node_count_part2 = 0;
    for (size_t i = 0; i < nodes_part1.size(); ++i)
    {
        auto& row_part1 = nodes_part1[i];
        auto& row_part2 = nodes_part2[i];
        for (size_t j = 0; j < row_part1.size(); ++j)
        {
            node_count_part1 += (row_part1[j] > 0);
            node_count_part2 += (row_part2[j] > 0);
        }
    }

    std::print("Part 1: {}\n", node_count_part1);
    std::print("Part 2: {}\n", node_count_part2);

    return 0;
}
