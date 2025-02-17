#include <print>
#include <fstream>
#include <istream>
#include <string>
#include <tuple>
#include <cstdio>
#include <vector>
#include <algorithm>

std::tuple<std::vector<int>, std::vector<int>> parse_input(std::istream& input)
{
    std::vector<int> list1;
    std::vector<int> list2;

    std::string line;

    while (std::getline(input, line))
    {
        int n1        = 0;
        int n2        = 0;
        int numValues = sscanf(line.data(), "%d   %d", &n1, &n2);
        if (numValues != 2)
        {
            std::print("Could not convert to int\n");
            continue;
        }

        list1.push_back(n1);
        list2.push_back(n2);
    }

    return {list1, list2};
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

    auto [list1, list2] = parse_input(inputFile);

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    int diff       = 0;
    int similarity = 0;
    for (std::size_t i = 0; i < list1.size(); ++i)
    {
        diff += std::abs(list1[i] - list2[i]);
        for (std::size_t j = 0; j < list2.size(); ++j)
        {
            if (list1[i] == list2[j])
            {
                similarity += list2[j];
            }
            else if (list2[j] > list1[i])
            {
                break;
            }
        }
    }

    std::print("Part 1: {}\n", diff);
    std::print("Part 2: {}\n", similarity);

    return 0;
}
