#include <print>
#include <fstream>
#include <istream>
#include <string>
#include <vector>
#include <cstdio>
#include <tuple>

struct Rule
{
    int first;
    int last;
};

using Numbers = std::vector<int>;
using Rules   = std::vector<Rule>;

std::tuple<Rules, std::vector<Numbers>> parse_input(std::istream& input)
{
    Rules                rules;
    std::vector<Numbers> numberList;

    for (std::string line; std::getline(input, line) && line.length() != 0;)
    {
        Rule rule;
        int  numValues = sscanf(line.data(), "%d|%d", &rule.first, &rule.last);
        if (numValues == 2)
        {
            rules.push_back(rule);
        }
    }

    for (std::string line; std::getline(input, line);)
    {
        Numbers numbers;
        int     number = 0;
        for (char c : line)
        {
            if (c == ',')
            {
                // store the number
                numbers.push_back(number);
                number = 0;
            }
            else
            {
                number = (number * 10) + (c - '0');
            }
        }
        // store the last number
        numbers.push_back(number);
        numberList.push_back(numbers);
    }
    return {rules, numberList};
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

    auto [rules, numberList] = parse_input(inputFile);

    int midCount      = 0;
    int fixedMidCount = 0;

    for (auto& numbers : numberList)
    {
        bool good              = false;
        bool fixed             = false;
        bool fixedARule        = false;
        int  timesCheckedRules = 0;
        // check the rules
        while (timesCheckedRules == 0 || fixedARule)
        {
            fixedARule = false;
            timesCheckedRules++;
            for (Rule rule : rules)
            {
                bool ruleGood   = true;
                bool lastFound  = false;
                int  lastIndex  = 0;
                int  firstIndex = 0;
                for (size_t i = 0; i < numbers.size(); ++i)
                {
                    int number = numbers[i];
                    if (number == rule.first)
                    {
                        firstIndex = i;
                        if (lastFound)
                        {
                            ruleGood = false;
                            break;
                        }
                    }
                    else if (number == rule.last)
                    {
                        lastIndex = i;
                        lastFound = true;
                    }
                }
                if (!ruleGood)
                {
                    int temp            = numbers[firstIndex];
                    numbers[firstIndex] = numbers[lastIndex];
                    numbers[lastIndex]  = temp;
                    fixed               = true;
                    fixedARule          = true;
                }
            }
            if (timesCheckedRules == 1 && !fixedARule)
            {
                good = true;
            }
        }
        if (good && !fixed)
        {
            midCount += numbers[numbers.size() / 2];
        }
        if (fixed)
        {
            fixedMidCount += numbers[numbers.size() / 2];
        }
    }

    std::print("Part 1: {}\n", midCount);
    std::print("Part 2: {}\n", fixedMidCount);

    return 0;
}
