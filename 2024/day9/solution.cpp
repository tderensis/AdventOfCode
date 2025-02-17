#include <print>
#include <fstream>
#include <string>
#include <vector>

struct DiskPartition
{
    int index;
    int used;
    int free;
};

using Disk = std::vector<DiskPartition>;

std::vector<Disk> parse_input(std::ifstream& input)
{
    std::vector<Disk> disks;

    for (std::string line; std::getline(input, line);)
    {
        bool          parsingUsedSize = true;
        Disk          disk;
        DiskPartition partition = {};
        for (char c : line)
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
        disks.push_back(disk);
    }

    return disks;
}

std::vector<int> defrag(Disk disk)
{
    std::vector<int> defrag_disk;
    bool             done = false;
    for (size_t i = 0; i < disk.size() - 1; ++i)
    {
        int used = disk[i].used;
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
                    if (i >= disk.size() - 1)
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
    for (size_t i = 0; i < defrag_disk.size(); ++i)
    {
        checksum += i * defrag_disk[i];
    }

    return checksum;
}

std::vector<int> defrag_whole(std::vector<DiskPartition> disk)
{
    std::vector<int> defrag_disk = {};

    for (int i = (int)disk.size() - 1; i >= 0; --i)
    {
        size_t j = 0;
        while (disk[j].index != disk[i].index)
        {
            if (disk[i].used <= disk[j].free)
            {
                disk[i - 1].free += disk[i].free + disk[i].used;
                disk[i].free = disk[j].free - disk[i].used;
                disk[j].free = 0;
                disk.insert(disk.begin() + j + 1, disk[i]);
                disk.erase(disk.begin() + i + 1);
                ++i;
                break;
            }
            ++j;
        }
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
    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto disks = parse_input(inputFile);

    for (auto& disk : disks)
    {
        auto defrag_disk = defrag(disk);

        auto checksum       = calculate_checksum(defrag_disk);
        auto checksum_whole = calculate_checksum(defrag_whole(disk));

        std::print("Part 1: {}\n", checksum);
        std::print("Part 2: {}\n", checksum_whole);
    }

    return 0;
}
