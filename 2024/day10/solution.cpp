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

constexpr int PEAK_HEIGHT = 9;

int reachable_peaks(std::vector<std::vector<Position>>& map, int x, int y)
{
    if (map[y][x].height == PEAK_HEIGHT)
    {
        if (map[y][x].visited == false)
        {
            // Uncomment for solution 1
            // map[y][x].visited = true;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    int count = 0;
    if (y > 0)
    {
        if (map[y][x].height + 1 == map[y - 1][x].height)
            count += reachable_peaks(map, x, y - 1);
    }
    if (y < map.size() - 1)
    {
        if (map[y][x].height + 1 == map[y + 1][x].height)
            count += reachable_peaks(map, x, y + 1);
    }
    if (x > 0)
    {
        if (map[y][x].height + 1 == map[y][x - 1].height)
            count += reachable_peaks(map, x - 1, y);
    }
    if (x < map[y].size() - 1)
    {
        if (map[y][x].height + 1 == map[y][x + 1].height)
            count += reachable_peaks(map, x + 1, y);
    }

    return count;
}

void clear_visited(std::vector<std::vector<Position>>& map)
{
    for (auto& row : map)
        for (auto& col : row)
            col.visited = false;
}

std::vector<Position> parse_map_row(std::string s)
{
    std::vector<Position> row;
    for (char c : s)
    {
        row.push_back({c - '0'});
    }

    return row;
}

int main(int argc, char* argv[])
{
    std::print("Day 10 Solution\n");

    std::string filename;

    if (argc < 2)
    {
        filename = "input.txt";
    }
    else
    {
        filename = argv[1];
    }

    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    std::string                        line;
    std::vector<std::vector<Position>> map;

    while (std::getline(inputFile, line))
    {
        auto map_row = parse_map_row(line);
        map.push_back(map_row);
    }

    int total_reachable_peaks = 0;
    for (int y = 0; y < map.size(); ++y)
    {
        for (int x = 0; x < map[y].size(); ++x)
        {
            if (map[y][x].height == 0)
            {
                total_reachable_peaks += reachable_peaks(map, x, y);
                clear_visited(map);
            }
        }
    }

    std::print("total reachable peaks {}\n", total_reachable_peaks);

    return 0;
}
