#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string_view>

int main(int argc, char* argv[])
{
    std::print("Day 7 Solution\n");

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
    std::string line;
    int64_t     total = 0;

    while (std::getline(inputFile, line))
    {
        std::vector<int64_t> numbers;
        int64_t              number = 0;
        int                  index  = 0;
        for (size_t i = 0; i < line.size(); ++i)
        {
            char c = line[i];
            if (c == ':')
            {
                index = i;
                break;
            }
            number = number * 10 + (c - '0');
        }

        int64_t result = number;
        number         = 0;

        ++index; // skip space
        ++index;

        for (size_t i = index; i < line.size(); ++i)
        {
            char c = line[i];
            if (c == ' ')
            {
                numbers.push_back(number);
                number = 0;
                continue;
            }
            number = number * 10 + (c - '0');
        }
        numbers.push_back(number);

        std::vector<int> ops(numbers.size() - 1, 2);
        while (true)
        {
            for (int& op : ops)
            {
                if (op == 2)
                {
                    op = 0;
                }
                else
                {
                    ++op;
                    break;
                }
            }

            int64_t test_result = numbers[0];
            for (size_t j = 0; j < ops.size(); ++j)
            {
                if (ops[j] == 0)
                {
                    test_result = test_result + numbers[j + 1];
                }
                else if (ops[j] == 1)
                {
                    test_result = test_result * numbers[j + 1];
                }
                else
                {
                    int64_t mag = 10;
                    int64_t x   = numbers[j + 1];
                    while (x /= 10)
                        mag *= 10;
                    test_result = test_result * mag + numbers[j + 1];
                }
            }
            if (result == test_result)
            {
                total += result;
                break;
            }
            bool cont = false;
            for (int op : ops)
            {
                if (op != 2)
                {
                    cont = true;
                    break;
                }
            }
            if (!cont)
                break;
        }
    }

    std::print("Total calibration result {}\n", total);

    return 0;
}
