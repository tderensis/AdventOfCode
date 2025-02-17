#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string_view>

struct Position
{
    int x;
    int y;
};

int main(int argc, char* argv[])
{
    std::print("Day 8 Solution\n");

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

    // Read the maze into a vector
    // Record the initial position and direction
    std::string                        line;
    std::vector<std::vector<Position>> positions;
    std::vector<std::vector<int>>      nodes;

    for (int i = 0; i < 256; ++i)
    {
        positions.push_back({});
    }

    int node_count = 0;
    int y          = 0;
    while (std::getline(inputFile, line))
    {
        std::vector<int> node_row;
        int              x = 0;
        for (char c : line)
        {
            if (c != '.')
            {
                positions[c].push_back({x, y});
            }
            ++x;
            node_row.push_back(0);
        }
        nodes.push_back(node_row);
        ++y;
    }

    for (int i = 0; i < positions.size(); ++i)
    {
        if (positions[i].size() <= 1)
            continue;

        for (int j = 0; j < positions[i].size(); ++j)
        {
            for (int k = 0; k < positions[i].size(); ++k)
            {
                int x1 = positions[i][j].x;
                int x2 = positions[i][k].x;
                int y1 = positions[i][j].y;
                int y2 = positions[i][k].y;

                nodes[y1][x1] = 1;

                if (j == k)
                    continue;

                int xn = x1 - x2 + x1;
                int yn = y1 - y2 + y1;

                while (xn >= 0 && xn < nodes[0].size() && yn >= 0 && yn < nodes.size())
                {
                    nodes[yn][xn] = 1;
                    x2            = x1;
                    y2            = y1;
                    x1            = xn;
                    y1            = yn;
                    xn            = x1 - x2 + x1;
                    yn            = y1 - y2 + y1;
                }
            }
        }
    }

    for (auto& row : nodes)
    {
        for (auto& col : row)
        {
            node_count += col;
        }
    }

    std::print("Antinode count {}\n", node_count);

    return 0;
}
