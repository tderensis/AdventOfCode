#include <print>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <numeric>
#include <utility>

using Stripe  = std::string;
using Stripes = std::vector<Stripe>;
using Towel   = std::string;
using Towels  = std::vector<Towel>;

std::tuple<Stripes, Towels> parse_input(std::istream& input)
{
    Stripes stripes;
    Towels  towels;

    std::string line;
    std::getline(input, line);

    Stripe stripe;
    for (char c : line)
    {
        if (c > 'a' && c <= 'z')
        {
            stripe.push_back(c);
        }
        else if (c == ',')
        {
            stripes.push_back(stripe);
            stripe.clear();
        }
    }

    for (std::string towel; std::getline(input, towel);)
    {
        if (towel.length() > 1)
            towels.push_back(towel);
    }

    return {stripes, towels};
}

void print(const auto& vec)
{
    for (const auto& v : vec)
    {
        std::print("{}\n", v);
    }
}

bool is_possible(const Stripes& stripes, const Towel& towel)
{
    std::unordered_set<std::size_t> visited;
    std::stack<std::size_t>         stack;
    stack.push(0);

    while (!stack.empty())
    {
        auto current = stack.top();
        stack.pop();

        if (current == towel.size())
        {
            return true;
        }
        if (visited.contains(current))
        {
            continue;
        }

        visited.insert(current);

        for (Stripe stripe : stripes)
        {
            if (current + stripe.length() > towel.size())
            {
                continue;
            }
            else if (towel.compare(current, stripe.length(), stripe) == 0)
            {
                stack.push(current + stripe.length());
            }
        }
    }

    return false;
}

uint64_t count_all_arrangements(
    std::unordered_map<std::size_t, uint64_t>& count_map, const Stripes& stripes,
    const Towel& towel, std::size_t index)
{
    if (index == towel.size())
    {
        return 1;
    }

    if (count_map.contains(index))
    {
        return count_map[index];
    }

    uint64_t sum = 0;
    for (const Stripe& stripe : stripes)
    {
        auto next_index = index + stripe.length();
        if (next_index > towel.size() + 1)
        {
            continue;
        }
        else if (towel.compare(index, stripe.length(), stripe) == 0)
        {
            sum += count_all_arrangements(count_map, stripes, towel, next_index);
        }
    }

    count_map[index] = sum;

    return sum;
}

uint64_t count_all_arrangements(const Stripes& stripes, const Towel& towel)
{
    std::unordered_map<std::size_t, uint64_t> count_map;
    auto result = count_all_arrangements(count_map, stripes, towel, 0);
    return result;
}

int main(int argc, char* argv[])
{
    std::print("Day 19 Solution\n");

    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto [stripes, towels] = parse_input(inputFile);

    int count = 0;
    for (auto towel : towels)
    {
        bool possible = is_possible(stripes, towel);
        count += possible;
    }
    std::print("Possible towels: {}\n", count);

    uint64_t all = 0;

    for (auto towel : towels)
    {
        auto c = count_all_arrangements(stripes, towel);
        std::print("{}: {}\n", c, towel);
        all += c;
    }

    std::print("All possible towels: {}\n", all);
    return 0;
}
