#include <print>
#include <fstream>
#include <istream>
#include <string>
#include <vector>

std::vector<std::vector<char>> parse_input(std::istream& input)
{
    std::vector<std::vector<char>> ws;

    for (std::string line; std::getline(input, line);)
    {
        std::vector<char> row;
        for (char c : line)
        {
            row.push_back(c);
        }
        ws.push_back(row);
    }
    return ws;
}

bool search_in_direction(
    const std::vector<std::vector<char>>& grid, size_t row, size_t col, std::string word, int x_dir,
    int y_dir)
{
    size_t x = row;
    size_t y = col;

    size_t m = grid.size();
    size_t n = grid[0].size();

    for (size_t k = 0; k < word.size(); k++)
    {
        if (x >= m || y >= n)
        {
            return false;
        }

        if (grid[x][y] != word[k])
        {
            return false;
        }

        x += x_dir;
        y += y_dir;
    }

    return true;
}

unsigned search_2d(
    const std::vector<std::vector<char>>& grid, size_t row, size_t col, std::string word)
{
    if (grid[row][col] != word[0])
    {
        return 0;
    }

    unsigned count = 0;

    int x[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int y[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (size_t dir = 0; dir < std::size(x); dir++)
    {
        if (search_in_direction(grid, row, col, word, x[dir], y[dir]))
        {
            ++count;
        }
    }

    return count;
}

unsigned count_xmas(const std::vector<std::vector<char>>& grid)
{
    size_t m = grid.size();
    size_t n = grid[0].size();

    unsigned count = 0;

    for (size_t i = 1; i < m - 1; ++i)
    {
        for (size_t j = 1; j < n - 1; ++j)
        {
            if (grid[i][j] == 'A')
            {
                char topLeft  = grid[i - 1][j - 1];
                char topRight = grid[i - 1][j + 1];
                char botLeft  = grid[i + 1][j - 1];
                char botRight = grid[i + 1][j + 1];
                if ((topLeft == 'M' && botRight == 'S') || (topLeft == 'S' && botRight == 'M'))
                {
                    if ((topRight == 'M' && botLeft == 'S') || (topRight == 'S' && botLeft == 'M'))
                    {
                        ++count;
                    }
                }
            }
        }
    }
    return count;
}

// This function calls search2D for each coordinate
unsigned search_word(const std::vector<std::vector<char>>& grid, std::string word)
{
    size_t m = grid.size();
    size_t n = grid[0].size();

    unsigned count = 0;

    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            count += search_2d(grid, i, j, word);
        }
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

    auto ws = parse_input(inputFile);

    std::print("Part 1: {}\n", search_word(ws, "XMAS"));
    std::print("Part 2: {}\n", count_xmas(ws));

    return 0;
}
