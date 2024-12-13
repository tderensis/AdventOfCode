#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string_view>

struct Rule
{
	int first;
	int last;
};

int main(int argc, char* argv[])
{
	std::print("Day5 Solution\n");

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
	std::vector<Rule> rules;
	std::string line;
	
	while (std::getline(inputFile, line) && line.length() != 0)
	{
	    Rule rule;
		int numValues = sscanf(line.data(), "%d|%d", &rule.first, &rule.last);
		if (numValues == 2)
		{
			rules.push_back(rule);
		}
	}

	int midCount = 0;
	int fixedMidCount = 0;

	while (std::getline(inputFile, line))
	{
		std::vector<int> numbers;
		int number = 0;
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

		bool good = false;
		bool fixed = false;
		bool fixedARule = false;
		int timesCheckedRules = 0;
		// check the rules
		while (timesCheckedRules == 0 || fixedARule)
		{
			fixedARule = false;
			timesCheckedRules++;
			for (Rule rule : rules)
			{
				bool ruleGood = true;
				bool lastFound = false;
				int lastIndex = 0;
				int firstIndex = 0;
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
					int temp = numbers[firstIndex];
					numbers[firstIndex] = numbers[lastIndex];
					numbers[lastIndex] = temp;
					fixed = true;
					fixedARule = true;
				}
			}
			if (timesCheckedRules == 1 && !fixedARule)
			{
				good = true;
			}
		}
		if (good && !fixed)
		{
			midCount += numbers[numbers.size()/2];
		}
		if (fixed)
		{
			fixedMidCount += numbers[numbers.size()/2];
		}
	}

	std::print("midCount {}\n", midCount);
	std::print("fixed midCount {}\n", fixedMidCount);
    return 0;
}
