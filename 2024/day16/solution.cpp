#include <print>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

constexpr char WALL = '#';
constexpr char START = 'S';
constexpr char END = 'E';
constexpr char EMPTY = '.';
constexpr char VISITED = '*';
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

std::tuple<Map, Position, Position> parse_input(std::istream& input)
{
    Map map;
	Position start;
	Position end;
	int line_count = 0;

    for (std::string line; std::getline(input, line);)
    {
	    MapRow row;
		for (char c : line)
		{
			if (c == START)
			{
				start = { (int)row.size(), line_count };
			}
			else if (c == END)
			{
				end = { (int)row.size(), line_count };
			}
			row.push_back(c);
		}
		map.push_back(row);
		++line_count;
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
        if (object_at(map, p) == EMPTY)
        {
			neighbor_visited = true;
            int h = heuristic(p, end);
			int g = (d == node.d) ? node.g + 1 : node.g + 1000 + 1;
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

void mark_path(Map& map, Position start, Position end, std::vector<std::vector<int>> scores, int score)
{
	auto& o = object_at(map, end);

	object_at(scores, end) = score;
	
	Direction d =
		(o == '^') ? Direction::down :
		(o == 'v') ? Direction::up :
		(o == '<') ? Direction::right :
		Direction::left;
	
	if (end == start)
	{
		o = 'O';
	}
	else
	{
		o = 'O';
		mark_path(map, start, move(end, d), scores, score);
	}
}

void clear_paths(Map& map)
{
    for (auto& row : map)
	{
		for (char& c : row)
		{
			if (c == '^' || c == 'v' || c == '<' || c == '>')
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

	std::print("no path found\n");
	return -1;
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

    auto [map, start, end] = parse_input(inputFile);
	auto original_map = map;
	print_map(map);

	auto direction = Direction::right;
	auto score = search(map, start, end, direction);

	std::vector<std::vector<int>> scores;
	for (auto& row : map)
	{
		std::vector<int> score_row;
		for (auto& c : row)
		{
			score_row.push_back(-1);
		}
		scores.push_back(score_row);
	}

	mark_path(map, start, end, scores, score);
	clear_paths(map);

	/*
	auto previous_start = start;
	while (true)
	{
		
		// Get the next value on the map
		auto next_direction = direction;
		for (Direction d :
		    {Direction::up, Direction::down, Direction::left, Direction::right})
		{
			// Make sure to not go backwards
			Position p = move(previous_start, d);
			if (previous_start == p)
				continue;
			
			if (object_at(map, p) == '0')
			{
			    next_direction = d;
			}
		}

		for (Direction d :
		    {Direction::up, Direction::down, Direction::left, Direction::right})
		{
			Position p = move(previous_start, d);
			if (opposite(direction) == d)
				continue;
			else if (next_direction == d)
				continue;
			
			if (object_at(map, p) == EMPTY)
			{
				// compare new and old distances
			    auto new_map = original_map;
			}
		}

		// Check score of empty paths
		previous_start = move(previous_start, next_direction);
		direction = next_direction;
	}
	*/
	print_map(map);

    std::print("Lowest Score: {}\n", score);
    return 0;
}
