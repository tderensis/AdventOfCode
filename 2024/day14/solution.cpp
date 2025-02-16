#include <print>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

struct Position
{
	int x;
	int y;
};

struct Velocity
{
	int x;
	int y;
};

struct Robot
{
    Position position;
	Velocity velocity;
};

using Robots = std::vector<Robot>;

Robots parse_input(std::istream& input)
{
    Robots robots;

    for (std::string line; std::getline(input, line);)
    {
		Robot robot;
        sscanf(
			line.data(),
			"p=%d,%d v=%d,%d",
			&robot.position.x,
			&robot.position.y,
			&robot.velocity.x,
			&robot.velocity.y);

		robots.push_back(robot);
	}

    return robots;
}

void print_robot(const Robot& robot)
{
    std::print(
		"p={},{} v={},{}\n",
		robot.position.x,
		robot.position.y,
		robot.velocity.x,
		robot.velocity.y);
}

void print_robots(const Robots& robots)
{
	for (const auto& robot : robots)
	{
		print_robot(robot);
	}
}

void print_robot_grid(const Robots& robots, int width, int height)
{
	std::vector<std::vector<int>> grid;
	for (int i = 0; i < height; ++i)
	{
		std::vector<int> row(width, 0);
		grid.push_back(row);
	}
	for (const auto& robot : robots)
	{
		++grid[robot.position.y][robot.position.x];
	}
	for (auto& row : grid)
	{
		for (auto& col : row)
		{
			std::print("{}", col == 0 ? '.' : (char)('0' + col));
		}
		std::print("\n");
	}
}

/**
   Move the robots
 */
void move_robots(Robots& robots, int width, int height)
{
    for (auto& robot : robots)
	{
	    robot.position.x =
			(robot.position.x + robot.velocity.x) % width;
		if (robot.position.x < 0)
		{
			robot.position.x += width;
		}
		robot.position.y =
			(robot.position.y + robot.velocity.y) % height;
		if (robot.position.y < 0)
		{
			robot.position.y += height;
		}
	}
}

int safety_factor(Robots& robots, int width, int height)
{
	int quadrant_top_left = 0;
	int quadrant_top_right = 0;
	int quadrant_bottom_left = 0;
	int quadrant_bottom_right = 0;
	for (auto& robot : robots)
	{
		if (robot.position.x < width/2 && robot.position.y < height/2)
		{
			++quadrant_top_left;
		}
		if (robot.position.x >= width - width/2 && robot.position.y < height/2)
		{
			++quadrant_top_right;
		}
		if (robot.position.x < width/2 && robot.position.y >= height -  height/2)
		{
			++quadrant_bottom_left;
		}
		if (robot.position.x >= width - width/2 && robot.position.y >= height -  height/2)
		{
			++quadrant_bottom_right;
		}
	}
	//std::print("tl {} tr {} bl {} br {} \n",
	//    quadrant_top_left,quadrant_top_right,quadrant_bottom_left,quadrant_bottom_right);
	if (quadrant_top_left == quadrant_top_right && quadrant_bottom_left == quadrant_bottom_right)
	{
		//print_robot_grid(robots, width, height);
		//getchar();
	}
	return quadrant_top_left * quadrant_top_right * quadrant_bottom_left * quadrant_bottom_right;
}

int variance(Robots& robots)
{
	int variance = 0;

	for (auto& robot : robots)
	{
		for (auto& other : robots)
		{
			variance += (
				abs(other.position.x - robot.position.x) +
				abs(other.position.y - robot.position.y))/2;
		}
	}
	return variance;
}

int main(int argc, char* argv[])
{
    std::print("Day 14 Solution\n");

    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto robots = parse_input(inputFile);
	int width = 0;
	int height = 0;
	int seconds = 10000;
	if (robots.size() <= 12)
	{
		// Test room is smaller
		width = 11;
		height = 7;
	}
	else
	{
		width = 101;
		height = 103;
	}
	//print_robots(robots);
	//print_robot_grid(robots, width, height);
	//std::print("initial safety factor {}\n", safety_factor(robots, width, height));
	int lowest_variance = 0x7FFFFFFF;
	int lowest_variance_time = 0;
    for (int i = 0; i < seconds; ++i)
    {
        move_robots(robots, width, height);
		if (i == 99)
		{
			std::print("safety factor at 100s {}\n",
					   safety_factor(robots, width, height));
		}
		
		//print_robots(robots);
		//std::print("time: {}\n", i+1);

		auto var = variance(robots);
		if (var < lowest_variance)
		{
			lowest_variance = var;
			// print_robot_grid(robots, width, height);
		    lowest_variance_time = i + 1;
		}
    }

    std::print("lowest variance time: {}\n", lowest_variance_time);
    return 0;
}
