#include <print>
#include <fstream>
#include <cstdint>
#include <string>
#include <array>
#include <vector>

struct Plot
{
    char plant;
    bool visited = false;
};

using PlotRow = std::vector<Plot>;
using Map     = std::vector<PlotRow>;

struct Region
{
    int     area;
    int     perimeter;
    int     corners = 0;
    Region& operator+=(const Region& rhs)
    {
        this->area += rhs.area;
        this->perimeter += rhs.perimeter;
        this->corners += rhs.corners;
        return *this; // return the result by reference
    }
};

struct Position
{
    size_t x;
    size_t y;
};

Map parse_map(std::istream& input)
{
    Map map;

    for (std::string line; std::getline(input, line);)
    {
        PlotRow row;
        for (char c : line)
        {
            row.push_back({c});
        }
        map.push_back(row);
    }

    return map;
}

Position up(Position p)
{
    return {p.x, p.y - 1};
}

Position down(Position p)
{
    return {p.x, p.y + 1};
}

Position left(Position p)
{
    return {p.x - 1, p.y};
}

Position right(Position p)
{
    return {p.x + 1, p.y};
}

bool in_bounds(const Map& map, Position p)
{
    return (p.y < map.size() && p.x < map[0].size());
}

int calculate_perimeter(const Map& map, Position p)
{
    int perimeter = 0;

    auto plant = map[p.y][p.x].plant;

    for (Position p_neighbor : {up(p), down(p), left(p), right(p)})
    {
        if (!in_bounds(map, p_neighbor) || plant != map[p_neighbor.y][p_neighbor.x].plant)
        {
            // up is different region
            ++perimeter;
        }
    }

    return perimeter;
}

void print_map(const Map& map)
{
    for (size_t y = 0; y < map.size(); ++y)
    {
        for (size_t x = 0; x < map[y].size(); ++x)
        {
            std::print("{}:{} ", map[y][x].plant, map[y][x].visited ? 1 : 0);
        }
        std::print("\n");
    }
}

int count_corners(const Map& map, Position p)
{
    int corners = 0;

    auto plant = map[p.y][p.x].plant;

    auto p_up                = up(p);
    auto p_down              = down(p);
    auto p_left              = left(p);
    auto p_right             = right(p);
    auto p_up_left           = up(left(p));
    auto p_up_right          = up(right(p));
    auto p_down_left         = down(left(p));
    auto p_down_right        = down(right(p));
    bool p_up_is_perimeter   = !in_bounds(map, p_up) || plant != map[p_up.y][p_up.x].plant;
    bool p_down_is_perimeter = !in_bounds(map, p_down) || plant != map[p_down.y][p_down.x].plant;
    bool p_left_is_perimeter = !in_bounds(map, p_left) || plant != map[p_left.y][p_left.x].plant;
    bool p_right_is_perimeter =
        !in_bounds(map, p_right) || plant != map[p_right.y][p_right.x].plant;

    corners += (p_up_is_perimeter && p_left_is_perimeter) ||
               ((!p_up_is_perimeter && !p_left_is_perimeter) &&
                plant != map[p_up_left.y][p_up_left.x].plant);
    corners += (p_up_is_perimeter && p_right_is_perimeter) ||
               ((!p_up_is_perimeter && !p_right_is_perimeter) &&
                plant != map[p_up_right.y][p_up_right.x].plant);
    corners += (p_down_is_perimeter && p_left_is_perimeter) ||
               ((!p_down_is_perimeter && !p_left_is_perimeter) &&
                plant != map[p_down_left.y][p_down_left.x].plant);
    corners += (p_down_is_perimeter && p_right_is_perimeter) ||
               ((!p_down_is_perimeter && !p_right_is_perimeter) &&
                plant != map[p_down_right.y][p_down_right.x].plant);
    return corners;
}

Region walk_region(Map& map, Position p)
{
    Region r = {
        .area = 1, .perimeter = calculate_perimeter(map, p), .corners = count_corners(map, p)};

    auto& plot   = map[p.y][p.x];
    plot.visited = true;

    for (Position p_neighbor : {up(p), down(p), left(p), right(p)})
    {
        if (!in_bounds(map, p_neighbor))
        {
            continue;
        }
        auto neighbor = map[p_neighbor.y][p_neighbor.x];

        if (neighbor.visited || neighbor.plant != plot.plant)
        {
            continue;
        }

        r += walk_region(map, p_neighbor);
    }
    return r;
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

    auto map = parse_map(inputFile);

    std::vector<Region> regions;

    for (size_t y = 0; y < map.size(); ++y)
    {
        for (size_t x = 0; x < map[y].size(); ++x)
        {
            if (!map[y][x].visited)
            {
                auto region = walk_region(map, {x, y});
                regions.push_back(region);
            }
        }
    }

    int total_price      = 0;
    int total_price_bulk = 0;
    for (auto region : regions)
    {
        total_price += region.area * region.perimeter;
        total_price_bulk += region.area * region.corners; // sides = corners
    }

    std::print("Part 1: {}\n", total_price);
    std::print("Part 2: {}\n", total_price_bulk);

    return 0;
}
