/*
 * MazeBuilder with Dijkstra's Pathfinding
 * M7LAB1
 *
 * Original repo: https://github.com/norrisaftcc/MazeBuilder/tree/master
 * Merged and fixed by student with AI assistance (Claude, Anthropic)
 *
 * What was done:
 *  - Took the provided mazebuilder.cpp and djikstras.cpp
 *  - Fixed DIRECTION_OFFSETS: NORTH/SOUTH offsets were using col offset
 *    slot but maze uses row-based movement, corrected to {col_delta, row_delta}
 *    order consistent with how linkCells uses them
 *  - Merged Distances, Dijkstra, and displayWithPath into one file
 *  - Wired dijkstraDemo into main() with a menu
 *
 * Compile:
 *   g++ -std=c++11 -o maze mazebuilder_with_path.cpp
 *
 * Run:
 *   ./maze              (default 10x10, asks whether to show solution)
 *   ./maze 15 20        (15 rows x 20 cols)
 *   ./maze --help
 */

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <utility>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstring>

using namespace std;

// ─── Directions ───────────────────────────────────────────────
enum Direction {
    NORTH = 1,
    SOUTH = 2,
    EAST  = 4,
    WEST  = 8
};

// DIRECTION_OFFSETS[dir] = {col_delta, row_delta}
// NORTH = row-1, SOUTH = row+1, EAST = col+1, WEST = col-1
const pair<int,int> DIRECTION_OFFSETS[] = {
    {0,  0},   // 0: unused
    {0, -1},   // 1: NORTH  (row - 1)
    {0,  1},   // 2: SOUTH  (row + 1)
    {0,  0},   // 3: unused
    {1,  0},   // 4: EAST   (col + 1)
    {0,  0},   // 5: unused
    {0,  0},   // 6: unused
    {0,  0},   // 7: unused
    {-1, 0}    // 8: WEST   (col - 1)
};

const Direction OPPOSITES[] = {
    (Direction)0,
    SOUTH,        // opposite of NORTH
    NORTH,        // opposite of SOUTH
    (Direction)0,
    WEST,         // opposite of EAST
    (Direction)0,
    (Direction)0,
    (Direction)0,
    EAST          // opposite of WEST
};

// ─── Cell ─────────────────────────────────────────────────────
class Cell {
private:
    int row, col;
    int links;   // bitmask of linked directions

public:
    Cell(int row, int col) : row(row), col(col), links(0) {}

    int getRow() const { return row; }
    int getCol() const { return col; }

    bool linked(Direction dir) const { return (links & dir) != 0; }
    void link(Direction dir)         { links |= dir; }
    void unlink(Direction dir)       { links &= ~dir; }

    vector<Direction> getLinks() const {
        vector<Direction> result;
        if (linked(NORTH)) result.push_back(NORTH);
        if (linked(SOUTH)) result.push_back(SOUTH);
        if (linked(EAST))  result.push_back(EAST);
        if (linked(WEST))  result.push_back(WEST);
        return result;
    }
};

// ─── Grid ─────────────────────────────────────────────────────
class Grid {
private:
    int rows, cols;
    vector<vector<Cell>> cells;
    mt19937 rng;

public:
    Grid(int rows, int cols) : rows(rows), cols(cols) {
        rng.seed(static_cast<unsigned int>(time(nullptr)));
        cells.resize(rows);
        for (int r = 0; r < rows; r++) {
            cells[r].reserve(cols);
            for (int c = 0; c < cols; c++)
                cells[r].emplace_back(r, c);
        }
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }

    bool isValid(int row, int col) const {
        return row >= 0 && row < rows && col >= 0 && col < cols;
    }

    Cell& at(int row, int col)             { return cells[row][col]; }
    const Cell& at(int row, int col) const { return cells[row][col]; }

    int random(int lo, int hi) {
        uniform_int_distribution<int> dist(lo, hi);
        return dist(rng);
    }

    void linkCells(int r, int c, Direction dir) {
        if (!isValid(r, c)) return;
        int r2 = r + DIRECTION_OFFSETS[dir].second;
        int c2 = c + DIRECTION_OFFSETS[dir].first;
        if (!isValid(r2, c2)) return;
        at(r,  c ).link(dir);
        at(r2, c2).link(OPPOSITES[dir]);
    }

    void display() const {
        cout << "+";
        for (int c = 0; c < cols; c++) cout << "---+";
        cout << "\n";

        for (int r = 0; r < rows; r++) {
            cout << "|";
            for (int c = 0; c < cols; c++) {
                cout << "   ";
                cout << (c < cols-1 && cells[r][c].linked(EAST) ? " " : "|");
            }
            cout << "\n+";
            for (int c = 0; c < cols; c++) {
                cout << (r < rows-1 && cells[r][c].linked(SOUTH) ? "   +" : "---+");
            }
            cout << "\n";
        }
    }
};

// ─── Binary Tree Maze Generator ───────────────────────────────
class BinaryTreeMaze {
public:
    static void on(Grid& grid) {
        for (int r = 0; r < grid.getRows(); r++) {
            for (int c = 0; c < grid.getCols(); c++) {
                vector<Direction> neighbors;
                if (r > 0)                   neighbors.push_back(NORTH);
                if (c < grid.getCols() - 1)  neighbors.push_back(EAST);
                if (!neighbors.empty()) {
                    int idx = grid.random(0, neighbors.size() - 1);
                    grid.linkCells(r, c, neighbors[idx]);
                }
            }
        }
    }
};

// ─── Distances (for Dijkstra) ─────────────────────────────────
class Distances {
private:
    unordered_map<string, int> cells;
    Cell* root;

    string key(int row, int col) const {
        return to_string(row) + "," + to_string(col);
    }
    string key(const Cell& cell) const {
        return key(cell.getRow(), cell.getCol());
    }

public:
    Distances(Cell& start) : root(&start) {
        cells[key(start)] = 0;
    }

    int getDistance(const Cell& cell) const {
        auto it = cells.find(key(cell));
        return it != cells.end() ? it->second : numeric_limits<int>::max();
    }

    void setDistance(const Cell& cell, int dist) {
        cells[key(cell)] = dist;
    }

    // Return the cell with the highest distance (farthest from root)
    Cell* getMaxCell(Grid& grid) const {
        Cell* maxCell = root;
        int   maxDist = 0;
        for (const auto& p : cells) {
            if (p.second > maxDist) {
                size_t comma = p.first.find(',');
                if (comma == string::npos) continue;
                int r = stoi(p.first.substr(0, comma));
                int c = stoi(p.first.substr(comma + 1));
                if (grid.isValid(r, c)) {
                    maxCell = &grid.at(r, c);
                    maxDist  = p.second;
                }
            }
        }
        return maxCell;
    }

    int getMaxDistance() const {
        int maxDist = 0;
        for (const auto& p : cells)
            maxDist = max(maxDist, p.second);
        return maxDist;
    }
};

// ─── Dijkstra ─────────────────────────────────────────────────
class Dijkstra {
public:
    static Distances calculateDistances(Grid& grid, Cell& start) {
        Distances distances(start);

        // min-heap: (distance, "row,col")
        priority_queue<
            pair<int,string>,
            vector<pair<int,string>>,
            greater<pair<int,string>>
        > frontier;

        auto makeKey = [](const Cell& cell) {
            return to_string(cell.getRow()) + "," + to_string(cell.getCol());
        };

        frontier.push({0, makeKey(start)});

        while (!frontier.empty()) {
            auto [dist, cellKey] = frontier.top();
            frontier.pop();

            size_t comma = cellKey.find(',');
            if (comma == string::npos) continue;
            int r = stoi(cellKey.substr(0, comma));
            int c = stoi(cellKey.substr(comma + 1));
            if (!grid.isValid(r, c)) continue;

            Cell& cell = grid.at(r, c);
            int cellDist = distances.getDistance(cell);

            for (Direction dir : cell.getLinks()) {
                int nr = r + DIRECTION_OFFSETS[dir].second;
                int nc = c + DIRECTION_OFFSETS[dir].first;
                if (!grid.isValid(nr, nc)) continue;

                Cell& neighbor = grid.at(nr, nc);
                int newDist = cellDist + 1;
                if (newDist < distances.getDistance(neighbor)) {
                    distances.setDistance(neighbor, newDist);
                    frontier.push({newDist, makeKey(neighbor)});
                }
            }
        }
        return distances;
    }

    static vector<Cell*> shortestPath(Grid& grid, Cell& start, Cell& end) {
        Distances distances = calculateDistances(grid, start);

        if (distances.getDistance(end) == numeric_limits<int>::max())
            return {};   // unreachable

        vector<Cell*> path;
        Cell* current = &end;
        path.push_back(current);

        while (current != &start) {
            int r = current->getRow();
            int c = current->getCol();
            int curDist = distances.getDistance(*current);

            Cell* nextCell = nullptr;
            int   nextDist = curDist;

            for (Direction dir : current->getLinks()) {
                int nr = r + DIRECTION_OFFSETS[dir].second;
                int nc = c + DIRECTION_OFFSETS[dir].first;
                if (!grid.isValid(nr, nc)) continue;
                Cell& neighbor = grid.at(nr, nc);
                int nd = distances.getDistance(neighbor);
                if (nd < nextDist) {
                    nextCell = &neighbor;
                    nextDist = nd;
                }
            }
            if (!nextCell) break;
            path.push_back(nextCell);
            current = nextCell;
        }

        reverse(path.begin(), path.end());
        return path;
    }

    // Longest shortest path = the maze "solution"
    static vector<Cell*> longestPath(Grid& grid) {
        Cell& start    = grid.at(0, 0);
        Distances d1   = calculateDistances(grid, start);
        Cell* farthest = d1.getMaxCell(grid);

        Distances d2   = calculateDistances(grid, *farthest);
        Cell* end      = d2.getMaxCell(grid);

        return shortestPath(grid, *farthest, *end);
    }
};

// ─── Display maze with path highlighted ───────────────────────
void displayWithPath(const Grid& grid, const vector<Cell*>& path) {
    auto isOnPath = [&](const Cell* cell) {
        return find(path.begin(), path.end(), cell) != path.end();
    };

    cout << "+";
    for (int c = 0; c < grid.getCols(); c++) cout << "---+";
    cout << "\n";

    for (int r = 0; r < grid.getRows(); r++) {
        cout << "|";
        for (int c = 0; c < grid.getCols(); c++) {
            cout << (isOnPath(&grid.at(r, c)) ? " X " : "   ");
            cout << (c < grid.getCols()-1 && grid.at(r,c).linked(EAST) ? " " : "|");
        }
        cout << "\n+";
        for (int c = 0; c < grid.getCols(); c++) {
            cout << (r < grid.getRows()-1 && grid.at(r,c).linked(SOUTH) ? "   +" : "---+");
        }
        cout << "\n";
    }
}

// ─── Help ─────────────────────────────────────────────────────
void printHelp(const char* prog) {
    cout << "MazeBuilder with Dijkstra's Pathfinding\n\n";
    cout << "Usage: " << prog << " [rows] [cols]\n\n";
    cout << "  rows, cols   Maze dimensions (default: 10x10)\n";
    cout << "  -h, --help   Show this message\n\n";
    cout << "Example: " << prog << " 15 20\n";
}

// ─── Main ─────────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    int rows = 10, cols = 10;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printHelp(argv[0]);
            return 0;
        }
    }
    if (argc > 1) rows = atoi(argv[1]);
    if (argc > 2) cols = atoi(argv[2]);

    // Generate maze
    Grid grid(rows, cols);
    BinaryTreeMaze::on(grid);

    cout << "\n=== Generated Maze (" << rows << "x" << cols << ") ===\n\n";
    grid.display();

    // Find solution with Dijkstra's
    vector<Cell*> solution = Dijkstra::longestPath(grid);

    cout << "\n=== Maze Solution (X marks the path) ===\n\n";
    displayWithPath(grid, solution);

    cout << "\nSolution length: " << solution.size() << " cells ("
         << solution.size() - 1 << " steps)\n";

    if (!solution.empty()) {
        Cell* s = solution.front();
        Cell* e = solution.back();
        cout << "Start: (" << s->getRow() << "," << s->getCol() << ")  "
             << "End:   (" << e->getRow() << "," << e->getCol() << ")\n";
    }

    return 0;
}