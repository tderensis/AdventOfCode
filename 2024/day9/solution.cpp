#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string_view>
struct DiskPartition
{
	int index;
    int used;
    int free;
};

std::vector<DiskPartition> parseDisk(std::string s)
{
    std::vector<DiskPartition> disk;

    bool parsingUsedSize = true;
    DiskPartition partition = {};
    for (char c : s)
    {
        if (parsingUsedSize)
        {
            partition.used = c - '0';
        }
        else
        {
            partition.free = c - '0';
            disk.push_back(partition);
			partition.index++;
        }
        parsingUsedSize = !parsingUsedSize;
    }

    partition.free = 0;
    disk.push_back(partition);

    return disk;
}

std::vector<int> defrag(std::vector<DiskPartition> disk)
{
    std::vector<int> defrag_disk;
    bool done = false;
    for (int i = 0; i < disk.size() - 1; ++i)
    {
        int used = disk[i].used;
        int free = disk[i].free;
        while (used--)
        {
            defrag_disk.push_back(i);
        }
        if (disk[i].free)
        {
            while (disk[i].free)
            {
                if (disk[disk.size() - 1].used)
                {
                    defrag_disk.push_back(disk.size() - 1);
                    --disk[disk.size() - 1].used;
                    --disk[i].free;
                }
                else
                {
                    disk.pop_back();
                    if(i >= disk.size() - 1)
                    {
                        done = true;
                        break;
                    }
                }
            }
        }
    }
    if (!done)
    {
        int used = disk.back().used;
        while (used--)
        {
            defrag_disk.push_back(disk.size() - 1);
        }
    }
    return defrag_disk;
}

int64_t calculate_checksum(std::vector<int> defrag_disk)
{
    int64_t checksum = 0;
    for (int i = 0; i < defrag_disk.size(); ++i)
    {
        checksum += i * defrag_disk[i];
    }

    return checksum;
}

std::vector<int> defrag_whole(std::vector<DiskPartition> disk)
{
    std::vector<int> defrag_disk = {};
    bool done = false;
    for (int i = disk.size() - 1; i >= 0; --i)
    {
		int j = 0;
        while (disk[j].index != disk[i].index)
		{
			if (disk[i].used <= disk[j].free)
			{
				disk[i-1].free += disk[i].free + disk[i].used;
				disk[i].free = disk[j].free - disk[i].used;
				disk[j].free = 0;
				disk.insert(disk.begin() + j + 1, disk[i]);
				disk.erase(disk.begin() + i + 1);
				++i;
				break;
			}
			++j;
        }
		//calculate_checksum(defrag_disk);
    }
    for (auto p : disk)
    {
        int used = p.used;
        while (used--)
        {
            defrag_disk.push_back(p.index);
        }
		int free = p.free;
		while (free--)
		{
			defrag_disk.push_back(0);
		}
    }

    return defrag_disk;
}

int main(int argc, char* argv[])
{
	std::print("Day 9 Solution\n");

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

	while (std::getline(inputFile, line))
	{
	    auto disk = parseDisk(line);

		auto defrag_disk = defrag(disk);

		auto checksum = calculate_checksum(defrag_disk);
		auto checksum_whole = calculate_checksum(defrag_whole(disk));

		std::print("cs {}\n", checksum);
		std::print("whole cs {}\n", checksum_whole);
	}
	
    return 0;
}
