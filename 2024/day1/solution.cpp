#include <print>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string_view>

int main(int argc, char* argv[])
{
	std::print("Day1 Solution\n");

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

	std::vector<int> list1;
	std::vector<int> list2;

	std::string line;
	
	while  (std::getline(inputFile, line))
	{
		int n1 = 0;
		int n2 = 0;
		int numValues = sscanf(line.data(), "%d   %d", &n1, &n2);
		if (numValues != 2)
		{
			std::print("Could not convert to int\n");
			return -1;
		}
		
		list1.push_back(n1);
		list2.push_back(n2);
	}

	std::sort(list1.begin(), list1.end());
	std::sort(list2.begin(), list2.end());

	int diff = 0;
	int similarity = 0;
	for (std::size_t i = 0; i < list1.size(); ++i)
	{
		diff += std::abs(list1[i] - list2[i]);
		for(std::size_t j = 0; j < list2.size(); ++j)
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

	std::print("List Diff: {}\n", diff);
	std::print("Similarity Score: {}\n", similarity);
													 
    return 0;
}
