#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string_view>

enum class Direction
{
    up,
    right,
    down,
    left,
    count,
};

struct Position
{
    size_t x;
    size_t y;
};

static constexpr char VISITED  = 'X';
static constexpr char OBSTACLE = '#';

Direction char_to_direction(char c)
{
    switch (c)
    {
    case '^': return Direction::up;
    case '>': return Direction::right;
    case 'v': return Direction::down;
    case '<': return Direction::left;
    default:  return Direction::count;
    }
}

Direction next_direction(Direction d)
{
    constexpr int MAX_DIR = static_cast<int>(Direction::count);
    return static_cast<Direction>((static_cast<int>(d) + 1) % MAX_DIR);
}

bool is_obstacle(char c)
{
    return c == OBSTACLE;
}

bool is_visited(char c)
{
    return c == VISITED;
}

Position next_position(Position p, Direction d)
{
    switch (d)
    {
    case Direction::up:    --p.y; return p;
    case Direction::right: ++p.x; return p;
    case Direction::down:  ++p.y; return p;
    case Direction::left:  --p.x; return p;
    default:               return p;
    }
}

bool is_position_in_bounds(Position p, size_t rows, size_t cols)
{
    return (p.y < rows && p.x < cols);
}

int main(int argc, char* argv[])
{
    std::print("Day 6 Solution\n");

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
    std::vector<std::vector<char>> maze;
    std::string                    line;
    Direction                      dir;
    Position                       pos{0, 0};

    while (std::getline(inputFile, line))
    {
        std::vector<char> maze_row;
        for (char c : line)
        {
            Direction d = char_to_direction(c);
            if (d != Direction::count)
            {
                dir   = d;
                pos.x = maze_row.size();
                pos.y = maze.size();
            }
            maze_row.push_back(c);
        }
        maze.push_back(maze_row);
    }

    // Starting position is visited
    Position  start_pos = pos;
    Direction start_dir = dir;

    maze[pos.y][pos.x] = VISITED;
    int visited        = 1;

    std::vector<std::vector<char>> start_maze = maze;

    while (true)
    {
        // Check next position
        Position next_pos = next_position(pos, dir);
        if (!is_position_in_bounds(next_pos, maze.size(), maze[0].size()))
        {
            break;
        }
        char next_char = maze[next_pos.y][next_pos.x];
        if (is_obstacle(next_char))
        {
            dir = next_direction(dir);
        }
        else
        {
            if (!is_visited(next_char))
            {
                ++visited;
            }
            maze[pos.y][pos.x] = VISITED;
            pos                = next_pos;
        }
    }

    int loop_count = 0;

    for (size_t row = 0; row < maze.size(); ++row)
    {
        for (size_t col = 0; col < maze[0].size(); ++col)
        {
            dir  = start_dir;
            pos  = start_pos;
            maze = start_maze;
            if (row == pos.y && col == pos.x)
            {
                continue;
            }
            if (is_obstacle(maze[row][col]))
            {
                continue;
            }
            maze[row][col]                = OBSTACLE;
            int  dir_change_without_visit = 0;
            bool visited_in_this_dir      = false;
            while (true)
            {
                // Check next position
                Position next_pos = next_position(pos, dir);
                if (!is_position_in_bounds(next_pos, maze.size(), maze[0].size()))
                {
                    break;
                }
                char next_char = maze[next_pos.y][next_pos.x];
                if (is_obstacle(next_char))
                {
                    if (!visited_in_this_dir)
                    {
                        ++dir_change_without_visit;
                    }
                    if (dir_change_without_visit == 4)
                    {
                        ++loop_count;
                        break;
                    }
                    dir                 = next_direction(dir);
                    visited_in_this_dir = false;
                }
                else
                {
                    if (!is_visited(next_char))
                    {
                        visited_in_this_dir = true;
                    }
                    maze[pos.y][pos.x] = VISITED;
                    pos                = next_pos;
                }
            }
        }
    }

    std::print("Visited {}\n", visited);
    std::print("Loop Count {}\n", loop_count);
    return 0;
}
