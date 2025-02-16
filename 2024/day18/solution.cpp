#include <print>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

constexpr char WALL = '#';
constexpr char START = 'S';
constexpr char END = 'E';
constexpr char EMPTY = '.';
constexpr char VISITED = 'O';
constexpr char PATH = 'p';

struct Position
{
	int x;
	int y;
	auto operator<=>(const Position&) const = default;
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

std::tuple<Map, Position, Position> parse_input(std::istream& input,
												int size, int bytes)
{
    Map map;
	Position start{0, 0};
	Position end{size - 1, size - 1};
	for (int i = 0; i < size; ++i)
	{
		MapRow row;
		for (int j = 0; j < size; ++j)
		{
			row.push_back(EMPTY);
		}
		map.push_back(row);
	}

	int line_count = 0;

    for (std::string line; std::getline(input, line);)
    {
	    int x;
		int y;
		sscanf(line.data(), "%d,%d", &x, &y);
		map[y][x] = WALL;
		++line_count;
		if (line_count == bytes)
			break;
	}

    return {map, start, end};
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

Direction opposite(Direction direction)
{
	switch (direction)
	{
	case Direction::up: return Direction::down;
	case Direction::down: return Direction::up;
	case Direction::left: return Direction::right;
	case Direction::right: return Direction::left;
	}
	return direction;
}

char dir_char(Direction direction)
{
	switch (direction)
	{
	case Direction::up: return '^';
	case Direction::down: return 'v';
	case Direction::left: return '<';
	case Direction::right: return '>';
	}
	return '^';
}

bool in_bounds(Map& map, Position p)
{
	return (p.y >= 0 && p.y < map.size() && p.x >= 0 && p.x < map[0].size());
}

auto& object_at(auto& map, Position p)
{
	return map[p.y][p.x];
}

int heuristic(Position start, Position end)
{
	return abs(start.x - end.x) + abs(start.y - end.y);
}

struct Node
{
	Position p;
	Direction d;
	int g;
	int h;
};

enum class State
{
    kEmpty,
    kObstacle,
    kClosed,
    kPath,
    kStart,
    kFinish
};

/*
Compare f-values of two cells.
*/
bool compare_nodes(const Node node1, const Node node2)
{
    return (node1.g + node1.h) > (node2.g + node2.h);
}

void sort_nodes(std::vector<Node>& nodes)
{
    sort(nodes.begin(), nodes.end(), compare_nodes);
}

bool visit_neighbors(
	Map& map,
	std::vector<Node>& open_nodes,
    const Node& node,
    Position end)
{
	bool neighbor_visited = false;
	for (Direction d :
        {Direction::up, Direction::down, Direction::left, Direction::right})
    {
		Position p = move(node.p, d);
        if (in_bounds(map, p) && object_at(map, p) == EMPTY)
        {
			neighbor_visited = true;
            int h = heuristic(p, end);
			int g = node.g + 1;
			open_nodes.push_back({p, d, g, h});
			object_at(map, p) =
				(d == Direction::up) ? '^' :
				(d == Direction::down) ? 'v' :
				(d == Direction::left) ? '<' :
				'>';
        }
    }
    return neighbor_visited;
}

void mark_path(Map& map, Position start, Position end, std::vector<Position>& path)
{
	auto& o = object_at(map, end);
	
	Direction d =
		(o == '^') ? Direction::down :
		(o == 'v') ? Direction::up :
		(o == '<') ? Direction::right :
		Direction::left;

	o = VISITED;
	path.push_back(end);
	if (end == start)
	{
		for (auto& row : map)
		{
			for (char& c : row)
			{
				if (c != WALL && c != VISITED)
				{
					c = EMPTY;
				}
			}
		}
	}
	else
	{
		mark_path(map, start, move(end, d), path);
	}
}

void clear_map(Map& map)
{
    for (auto& row : map)
	{
		for (char& c : row)
		{
			if (c != WALL)
			{
				c = EMPTY;
			}
		}
	}
}

auto search(Map& map, Position start, Position end, Direction direction)
{
	std::vector<Node> open_nodes{};

    open_nodes.push_back({start, direction, 0, heuristic(start, end)});
    object_at(map, start) = dir_char(direction);
	object_at(map, end) = EMPTY;

    while (open_nodes.size() > 0)
    {
        sort_nodes(open_nodes);
        Node current_node = open_nodes.back();
        open_nodes.pop_back();

        if (current_node.p == end)
        {
            return current_node.g;
        }
        visit_neighbors(map, open_nodes, current_node, end);
    }

	return -1;
}

int main(int argc, char* argv[])
{
    std::print("Day 18 Solution\n");

    std::string   filename = argc < 2 ? "input.txt" : argv[1];
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::print("Couldn't open {}\n", filename);
        return -1;
    }

    auto [map, start, end] = parse_input(inputFile, 70+1, 1024);

	auto direction = Direction::right;
	auto score = search(map, start, end, direction);

	std::vector<Position> path;
	mark_path(map, start, end, path);
	print_map(map);
	clear_map(map);

    std::print("steps: {}\n", score);

	for (std::string line; std::getline(inputFile, line);)
    {
		int x;
		int y;
		sscanf(line.data(), "%d,%d", &x, &y);
		map[y][x] = WALL;
		if (path.end() == std::find(path.begin(), path.end(), Position{x, y}))
		{
			// no collision, nothing to do
		}
	    else if (-1 == search(map, start, end, direction))
		{
			std::print("Failed at {},{}\n", x, y);
			break;
		}
		else
		{
			path.clear();
			mark_path(map, start, end, path);
			clear_map(map);
		}
	}
    return 0;
}
