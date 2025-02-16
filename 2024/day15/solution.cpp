#include <print>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

constexpr char WALL = '#';
constexpr char BOX = 'O';
constexpr char BOX_LEFT = '[';
constexpr char BOX_RIGHT = ']';
constexpr char ROBOT = '@';
constexpr char EMPTY = '.';

struct Position
{
	int x;
	int y;
};

enum class Direction
{
	up,
	down,
	left,
	right
};

using MapRow = std::vector<char>;
using Map = std::vector<MapRow>;
using Directions = std::vector<Direction>;

std::tuple<Map, Directions> parse_input(std::istream& input)
{
    Map map;
	Directions directions;

    for (std::string line; std::getline(input, line);)
    {
		if (line[0] == WALL)
		{
			MapRow row;
			for (char c : line)
			{
				row.push_back(c);
			}
			map.push_back(row);
		}
		else
		{
			for (char c : line)
			{
				switch (c)
				{
				case '^': directions.push_back(Direction::up); break;
				case 'v': directions.push_back(Direction::down); break;
				case '<': directions.push_back(Direction::left); break;
				case '>': directions.push_back(Direction::right); break;
				}
			}
		}
	}

    return {map, directions};
}

void print_map(const Map& map)
{
    for (const auto& row : map)
	{
		for (char c : row)
		{
			std::print("{}", c);
		}
		std::print("\n");
	}
}

Position up(Position p)
{
	return { p.x, p.y - 1};
}

Position down(Position p)
{
	return { p.x, p.y + 1};
}

Position left(Position p)
{
	return { p.x - 1, p.y};
}

Position right(Position p)
{
	return { p.x + 1, p.y};
}

Position move(Position position, Direction direction)
{
	switch (direction)
	{
	case Direction::up: return up(position);
	case Direction::down: return down(position);
	case Direction::left: return left(position);
	case Direction::right: return right(position);
	}
	return position;
}

auto& object_at(Map& map, Position p)
{
	return map[p.y][p.x];
}

bool is_box(char c)
{
	return c == BOX || c == BOX_LEFT || c == BOX_RIGHT;
}

bool can_move_object(Map& map, Position position, Direction direction)
{
	auto& object = object_at(map, position);
	auto  new_position = move(position, direction);
	auto& object_at_new_position = object_at(map, new_position);
	if (object_at_new_position == EMPTY)
	{
		return true;
	}
	else if (object_at_new_position == WALL)
	{
		return false;
	}

	return can_move_object(map, new_position, direction);
}
bool move_object(Map& map, Position position, Direction direction)
{
	auto& object = object_at(map, position);
    auto new_position = move(position, direction);
	auto& object_at_new_position = object_at(map, new_position);
	if (object_at_new_position == EMPTY)
	{
		object_at_new_position = object;
		object = EMPTY;
		return true;
	}
	else if (is_box(object_at_new_position) && can_move_object(map, new_position, direction))
	{
		move_object(map, new_position, direction);
		object_at_new_position = object;
		object = EMPTY;
		return true;
	}
	else
	{
		return false; // must be a wall (this will never be a robot)
	}
}
/*
bool move_object(Map& map, Position position, Direction direction)
{
    Position new_position = move(position, direction);
	auto& object = object_at(map, position);
	auto& object_at_new_position = object_at(map, new_position);
	if (object_at_new_position == EMPTY)
	{
		object_at_new_position = object;
		object = EMPTY;
		return true;
	}
	else if (object_at_new_position == BOX)
	{
	    if (move_object(map, new_position, direction))
		{
			object_at_new_position = object;
			object = EMPTY;
			return true;
		}
		return false;
	}
	else
	{
		return false; // must be a wall (this will never be a robot)
	}
}
*/


bool can_move_big_object(Map& map, Position position, Direction direction)
{
	auto& object = object_at(map, position);

	if (object == EMPTY)
	{
		return true;
	}
	else if (is_box(object))
	{
		if (direction == Direction::up || direction == Direction::down)
		{
			Position new_position_1 = move(position, direction);
			Position new_position_2 = (object == BOX_LEFT) ?
				move(new_position_1, Direction::right) :
				move(new_position_1, Direction::left);

			auto& object_at_new_position_1 = object_at(map, new_position_1);
			auto& object_at_new_position_2 = object_at(map, new_position_2);

			if (object_at_new_position_1 == EMPTY &&
				object_at_new_position_2 == EMPTY)
			{
				return true;
			}

			return can_move_big_object(map, new_position_1, direction) &&
				can_move_big_object(map, new_position_2, direction);
		}
		return true; // we can always at least try to move left/right
	}
	else
	{
		return false; // must be a wall (this will never be a robot)
	}
}

bool move_big_object(Map& map, Position position, Direction direction)
{
	auto& object = object_at(map, position);

	if (object == ROBOT)
	{
		Position new_position = move(position, direction);
		auto& object_at_new_position = object_at(map, new_position);
		if (object_at_new_position == EMPTY)
		{
			object_at_new_position = object;
			object = EMPTY;
			return true;
		}
		else if (is_box(object_at_new_position))
		{
			if (move_big_object(map, new_position, direction))
			{
				if (direction == Direction::up || direction == Direction::down)
				{
					auto& object_side = (object_at_new_position == BOX_LEFT) ?
						object_at(map, right(position)) :
						object_at(map, left(position));
					//object_side = EMPTY;
				}
				object_at_new_position = object;
				object = EMPTY;
				return true;
			}
			return false;
		}
		else
		{
			return false; // must be a wall (this will never be a robot)
		}
	}
	else if (is_box(object))
	{
		if (direction == Direction::up || direction == Direction::down)
		{
			Position new_position_1 = move(position, direction);
			Position new_position_2 = (object == BOX_LEFT) ?
				move(new_position_1, Direction::right) :
				move(new_position_1, Direction::left);

			auto& object_at_new_position_1 = object_at(map, new_position_1);
			auto& object_at_new_position_2 = object_at(map, new_position_2);

			if (object_at_new_position_1 == EMPTY &&
				object_at_new_position_2 == EMPTY)
			{
				object_at_new_position_1 = object;
				object_at_new_position_2 = (object == BOX_LEFT) ?
					BOX_RIGHT : BOX_LEFT;
				
				auto& other_box = (object == BOX_LEFT) ?
					object_at(map, move(position, Direction::right)) :
					object_at(map, move(position, Direction::left));
				object = EMPTY;
				other_box = EMPTY;
				return true;
			}
			else if (object == object_at_new_position_1)
			{
				if (move_big_object(map, new_position_1, direction))
				{
				    object_at_new_position_1 = object;
					object_at_new_position_2 = (object == BOX_LEFT) ?
						BOX_RIGHT : BOX_LEFT;
					
					auto& other_box = (object == BOX_LEFT) ?
						object_at(map, move(position, Direction::right)) :
						object_at(map, move(position, Direction::left));
					object = EMPTY;
					other_box = EMPTY;
					return true;
				}
				return false;
			}
			else if (can_move_big_object(map, new_position_1, direction) &&
					 can_move_big_object(map, new_position_2, direction))
			{
			    move_big_object(map, new_position_1, direction);
				move_big_object(map, new_position_2, direction);
				object_at_new_position_1 = object;
				object_at_new_position_2 = (object == BOX_LEFT) ?
					BOX_RIGHT : BOX_LEFT;
				
				auto& other_box = (object == BOX_LEFT) ?
					object_at(map, move(position, Direction::right)) :
					object_at(map, move(position, Direction::left));
				object = EMPTY;
				other_box = EMPTY;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			// moving left/right
			auto side_position = move(move(position, direction), direction);
			auto& side_object = object_at(map, side_position);
			auto& other_box = object_at(map, move(position, direction));
			if (side_object == EMPTY)
			{
				side_object = other_box;
				other_box = object;
				object = EMPTY;
				return true;
			}
			else {
				if (move_big_object(map, side_position, direction))
				{
					side_object = other_box;
					other_box = object;
					object = EMPTY;
					return true;
				}
				return false;
			}
			return false;
		}
	}
	else
	{
		return false; // must be a wall (this will never be a robot)
	}
}

Position find_robot(Map& map)
{
    for (int row = 0; row < map.size(); ++row)
	{
		for (int col = 0; col < map[row].size(); ++col)
		{
			Position p = {col, row};
			if (object_at(map, p) == ROBOT)
			{
				return p;
			}
		}
	}
	return {};
}

int sum_gps_coordinates(Map& map)
{
	int sum = 0;
	for (int row = 0; row < map.size(); ++row)
	{
		for (int col = 0; col < map[row].size(); ++col)
		{
			Position p = {col, row};
			auto& object = object_at(map, p);
			if (object == BOX || object == BOX_LEFT)
			{
				sum += row * 100 + col;
			}
		}
	}
	return sum;
}

int main(int argc, char* argv[])
{
    std::print("Day 15 Solution\n");

    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto [original_map, directions] = parse_input(inputFile);
	Map map{original_map};
	auto robot = find_robot(map);

	for (auto direction : directions)
	{
		if (move_object(original_map, robot, direction))
		{
			robot = move(robot, direction);
		}
		//std::print("{}\n", (int)direction);
		//print_map(original_map);
		//getchar();
    }

    std::print("GPS coordinate sum: {}\n", sum_gps_coordinates(original_map));

	Map big_map;
    for (int row = 0; row < map.size(); ++row)
	{
		MapRow map_row;
		for (int col = 0; col < map[row].size(); ++col)
		{
		    if (map[row][col] == WALL)
			{
				map_row.push_back(WALL);
				map_row.push_back(WALL);
			}
			else if (map[row][col] == BOX)
			{
				map_row.push_back(BOX_LEFT);
				map_row.push_back(BOX_RIGHT);
			}
			else if (map[row][col] == EMPTY)
			{
				map_row.push_back(EMPTY);
				map_row.push_back(EMPTY);
			}
			else if (map[row][col] == ROBOT)
			{
				map_row.push_back(ROBOT);
				map_row.push_back(EMPTY);
			}
		}
		big_map.push_back(map_row);
	}
	
	robot = find_robot(big_map);

	for (auto direction : directions)
	{
		if (move_big_object(big_map, robot, direction))
		{
			robot = move(robot, direction);
		}
		//std::print("{}\n", (int)direction);
		//print_map(big_map);
		//getchar();
    }
	std::print("GPS coordinate sum: {}\n", sum_gps_coordinates(big_map));

    return 0;
}
