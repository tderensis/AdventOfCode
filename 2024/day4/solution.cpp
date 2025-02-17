#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string_view>

int search2D(std::vector<std::vector<char>> grid, int row, int col, std::string word)
{
    int m     = grid.size();
    int n     = grid[0].size();
    int count = 0;

    // return false if the given coordinate
    // does not match with first index char.
    if (grid[row][col] != word[0])
        return 0;

    int len = word.size();

    // x and y are used to set the direction in which
    // word needs to be searched.
    std::vector<int> x = {-1, -1, -1, 0, 0, 1, 1, 1};
    std::vector<int> y = {-1, 0, 1, -1, 1, -1, 0, 1};

    // This loop will search in all the 8 directions
    // one by one. It will return true if one of the
    // directions contain the word.
    for (int dir = 0; dir < 8; dir++)
    {

        // Initialize starting point for current direction
        int k, currX = row + x[dir], currY = col + y[dir];

        // First character is already checked, match remaining
        // characters
        for (k = 1; k < len; k++)
        {

            // break if out of bounds
            if (currX >= m || currX < 0 || currY >= n || currY < 0)
                break;

            if (grid[currX][currY] != word[k])
                break;

            //  Moving in particular direction
            currX += x[dir], currY += y[dir];
        }

        // If all character matched, then value of must
        // be equal to length of word
        if (k == len)
            ++count;
    }

    return count;
}

int countXMAS(std::vector<std::vector<char>> grid)
{
    int m     = grid.size();
    int n     = grid[0].size();
    int count = 0;

    for (std::size_t i = 1; i < m - 1; ++i)
    {
        for (std::size_t j = 1; j < n - 1; ++j)
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
int searchWord(std::vector<std::vector<char>> grid, std::string word)
{
    int m = grid.size();
    int n = grid[0].size();

    int count = 0;

    for (std::size_t i = 0; i < m; ++i)
    {
        for (std::size_t j = 0; j < n; ++j)
        {
            count += search2D(grid, i, j, word);
        }
    }
    return count;
}

int main(int argc, char* argv[])
{
    std::print("Day4 Solution\n");

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

    // Read the wordsearch into a vector
    std::vector<std::vector<char>> ws;
    std::string                    line;

    while (std::getline(inputFile, line))
    {
        std::vector<char> row;
        for (char c : line)
        {
            row.push_back(c);
        }
        ws.push_back(row);
    }

    // do the search
    int count = 0;

    std::vector<std::string> words = {
        "XMAS",
    };

    for (auto word : words)
    {
        count += searchWord(ws, word);
    }

    std::print("word count {}\n", count);

    std::print("x-mas count {}\n", countXMAS(ws));
    return 0;
}
