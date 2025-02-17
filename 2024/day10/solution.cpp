#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string_view>

struct Position
{
    int  height;
    bool visited = false;
};

using Map = std::vector<std::vector<Position>>;

Map parse_input(std::ifstream& input)
{
    Map map;
    for (std::string line; std::getline(input, line);)
    {
        std::vector<Position> row;
        for (char c : line)
        {
            row.push_back({c - '0'});
        }
        map.push_back(row);
    }
    return map;
}

constexpr int PEAK_HEIGHT = 9;

unsigned reachable_peaks(std::vector<std::vector<Position>>& map, size_t x, size_t y, bool part1)
{
    if (map[y][x].height == PEAK_HEIGHT)
    {
        if (map[y][x].visited == false)
        {
            if (part1)
            {
                map[y][x].visited = true;
            }
            return 1;
        }
        else
        {
            return 0;
        }
    }
    unsigned count = 0;
    if (y > 0)
    {
        if (map[y][x].height + 1 == map[y - 1][x].height)
        {
            count += reachable_peaks(map, x, y - 1, part1);
        }
    }
    if (y < map.size() - 1)
    {
        if (map[y][x].height + 1 == map[y + 1][x].height)
        {
            count += reachable_peaks(map, x, y + 1, part1);
        }
    }
    if (x > 0)
    {
        if (map[y][x].height + 1 == map[y][x - 1].height)
        {
            count += reachable_peaks(map, x - 1, y, part1);
        }
    }
    if (x < map[y].size() - 1)
    {
        if (map[y][x].height + 1 == map[y][x + 1].height)
        {
            count += reachable_peaks(map, x + 1, y, part1);
        }
    }

    return count;
}

void clear_visited(std::vector<std::vector<Position>>& map)
{
    for (auto& row : map)
        for (auto& col : row)
            col.visited = false;
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

    auto map = parse_input(inputFile);

    unsigned total_reachable_peaks_part1 = 0;
    for (size_t y = 0; y < map.size(); ++y)
    {
        for (size_t x = 0; x < map[y].size(); ++x)
        {
            if (map[y][x].height == 0)
            {
                total_reachable_peaks_part1 += reachable_peaks(map, x, y, true);
                clear_visited(map);
            }
        }
    }
    unsigned total_reachable_peaks_part2 = 0;
    for (size_t y = 0; y < map.size(); ++y)
    {
        for (size_t x = 0; x < map[y].size(); ++x)
        {
            if (map[y][x].height == 0)
            {
                total_reachable_peaks_part2 += reachable_peaks(map, x, y, false);
                clear_visited(map);
            }
        }
    }

    std::print("Part 1: {}\n", total_reachable_peaks_part1);
    std::print("Part 2: {}\n", total_reachable_peaks_part2);

    return 0;
}
