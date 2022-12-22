#include <array>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

static int row_size = 200;
using matrix = std::vector<std::vector<int>>;

class point : public std::pair<int, int> {
public:
    static size_t pair_hash(const point &p) {
        size_t hash = p.first + p.second * row_size;
        return hash;
    }

    using hashset = std::unordered_set<point, decltype(&pair_hash)>;
    using queue = std::deque<point>;

    static void consider_point(point &next_point, hashset &visited, queue &next_iter) {
        if (visited.find(next_point) == visited.end()) {
            visited.insert(next_point);
            next_iter.push_back(next_point);
        }
    }

    point() {}
    point(int x, int y) : std::pair<int, int>(x, y) {}
    point(std::pair<unsigned long, unsigned long> p) {
        this->first = p.first;
        this->second = p.second;
    }

    const std::pair<int, int> to_pair() const {
        return {this->first, this->second};
    }

    bool can_go(const matrix &grid, const point &next) const {
        return grid[second][first] >= grid[next.second][next.first] - 1;
    }

    std::array<point, 4> directions() {
        return {
            point(first, second + 1),
            point(first - 1, second),
            point(first, second - 1),
            point(first + 1, second),
        };
    }

    bool is_valid(const matrix &grid) const {
        return first >= 0 && first < grid[0].size() && second >= 0 &&
               second < grid.size();
    }

    int propagate(
            const matrix& grid,
            const point& end,
            hashset& visited,
            std::function<bool(const matrix& grid, const point& current, const point& end)> comp,
            std::function<bool(const matrix& grid, const point& current, const point& direction)> filter
        ) {
        int distance = 0;
        queue next_iter, next;

        next_iter.push_back(*this);
        visited.insert(*this);

        while (!next_iter.empty()) {
            next.swap(next_iter);
            distance++;
            while (!next.empty()) {
                auto current = next.front();
                std::cout << current.first << " " << current.second << std::endl;

                if (comp(grid, current, end))
                    return distance - 1;

                auto directions = current.directions(); //up left down right
                for (auto direction : directions)
                    if (direction.is_valid(grid))
                        if (filter(grid, current, direction))
                            consider_point(direction, visited, next_iter);

                next.pop_front();
            }
        }

        return -1;
    }
};


// using point = std::pair<int, int>;
using hashset = std::unordered_set<point, decltype(&point::pair_hash)>;
using queue = std::deque<point>;

void read_file(matrix &grid, point &start, point &end) {
    std::ifstream file("input", std::ios_base::in);
    std::string line;
    while (file >> line) {

        std::vector<int> row;
        for (char c : line) {
            int elevation = 0;
            if (c == 'S')
                start = std::make_pair(row.size(), grid.size());
            else if (c == 'E') {
                end = std::make_pair(row.size(), grid.size());
                elevation = 'z' - 'a';
            } else {
                elevation = c - 'a';
            }
            row.push_back(elevation);
        }
        grid.push_back(row);
    }
    row_size = grid[0].size();
    file.close();
}


int main() {
    matrix grid;
    point start, end;
    hashset visited(0, point::pair_hash);
    read_file(grid, start, end);

    // first challenge
    int result = start.propagate(
        grid,
        end,
        visited,
        [](const matrix& grid, const point& current, const point& end) { return current == end; },
        [](const matrix& grid, const point& current, const point& dir) { return current.can_go(grid, dir); }
    );

    std::cout << std::endl << "FIRST RESULT = " << result << std::endl;

    // second challenge
    visited.clear();
    result = end.propagate(
        grid,
        end,
        visited,
        [](const matrix& grid, const point& current, const point& end) { return grid[current.second][current.first] == 0; },
        [](const matrix& grid, const point& current, const point& dir) { return dir.can_go(grid, current); }
    );
    std::cout << std::endl << "SECOND RESULT = " << result << std::endl;

    return 0;
}
