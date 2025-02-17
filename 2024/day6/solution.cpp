#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string_view>
#include <ranges>

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

using Maze = std::vector<std::vector<char>>;

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

std::tuple<Maze, Position, Direction> parse_input(std::istream& input)
{

    Maze      maze;
    Direction dir;
    Position  pos{0, 0};

    for (std::string line; std::getline(input, line);)
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

    return {maze, pos, dir};
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
    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto [maze, pos, dir] = parse_input(inputFile);

    Position  start_pos = pos;
    Direction start_dir = dir;

    maze[pos.y][pos.x] = VISITED; // Starting position is visited
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
        for (const auto& [row_idx, col] :
             std::views::enumerate(std::views::iota(0uz, maze[0].size())))
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

    std::print("Part 1: {}\n", visited);
    std::print("Part 2: {}\n", loop_count);

    return 0;
}
