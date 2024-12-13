#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string_view>

bool levelIsSafe(std::vector<int> level)
{
	int first = level[0];
	int second = level[1];
	int difference = abs(first - second);

	if (difference < 1 || difference > 3)
	{
		return false;
	}

	bool previousAscending = second > first;
	int previous = second;
	for (std::size_t i = 2; i < level.size(); ++i)
	{
		int current = level[i];
		bool currentAscending = current > previous;

		if (previousAscending != currentAscending)
		{
		    return false;
		}
		int difference = abs(current - previous);
		if (difference < 1 || difference > 3)
		{
			return false;
		}
		previousAscending = currentAscending;
		previous = current;
	}
	return true;
}

int main(int argc, char* argv[])
{
	std::print("Day2 Solution\n");

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

	std::string line;
	int numSafe = 0;
	int numSafeOneRemoved = 0;
	
	while  (std::getline(inputFile, line))
	{
		std::stringstream stream(line);

		std::vector<int> numberList;
		while (!stream.eof())
		{
			int number;
			std::string s;
			stream >> s;
			if (std::stringstream(s) >> number)
			{
				numberList.push_back(number);
			}
		}

		if (levelIsSafe(numberList))
		{
			++numSafe;
			++numSafeOneRemoved;
		}
		else
		{
			for (std::size_t i = 0; i < numberList.size(); ++i)
			{
				std::vector<int> levelListOneRemoved = numberList;
				levelListOneRemoved.erase(levelListOneRemoved.begin() + i);
				if (levelIsSafe(levelListOneRemoved))
				{
					++numSafeOneRemoved;
					break;
				}
			}
		}
	}

	std::print("Num Safe Reports: {}\n", numSafe);
	std::print("Num Safe Reports One Removed: {}\n", numSafeOneRemoved);
													 
    return 0;
}
