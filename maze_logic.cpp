#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <set>
#include <map>

using namespace std;

// 1 = Wall, 0 = Path
const int WALL = 1;
const int PATH = 0;

struct Point {
    int x, y;
    
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Node for A* algorithm
struct Node {
    Point pos;
    int g; // Cost from start
    int h; // Heuristic to end
    int f; // Total cost
    Point parent;
    
    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

class Maze {
    int size;
    int rows, cols;
    vector<vector<int>> grid;

public:
    Maze(int n) {
        size = n;
        // We use 2*n+1 to ensure walls surround cells
        rows = 2 * size + 1;
        cols = 2 * size + 1;
        grid.resize(rows, vector<int>(cols, WALL));
    }

    void generate() {
        srand(time(0));
        stack<Point> s;
        
        // Start at (1,1)
        grid[1][1] = PATH;
        s.push({1, 1});

        while (!s.empty()) {
            Point current = s.top();
            int x = current.x;
            int y = current.y;

            // Neighbors: Up, Down, Left, Right (distance of 2)
            vector<Point> neighbors;
            int dx[] = {0, 0, 2, -2};
            int dy[] = {2, -2, 0, 0};

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx > 0 && nx < rows - 1 && ny > 0 && ny < cols - 1 && grid[nx][ny] == WALL) {
                    neighbors.push_back({nx, ny});
                }
            }

            if (!neighbors.empty()) {
                // Choose random neighbor
                Point next = neighbors[rand() % neighbors.size()];
                
                // Remove wall between current and next
                grid[(x + next.x) / 2][(y + next.y) / 2] = PATH;
                grid[next.x][next.y] = PATH;

                s.push(next);
            } else {
                s.pop();
            }
        }
        
        // Ensure start and end are open
        grid[0][1] = PATH; // Entry
        grid[rows - 1][cols - 2] = PATH; // Exit
        
        // This creates loops and alternative routes
        int numExtraOpenings = (rows * cols) / 20; 
        for (int i = 0; i < numExtraOpenings; i++) {
            int rx = 1 + rand() % (rows - 2);
            int ry = 1 + rand() % (cols - 2);
            
            if (grid[rx][ry] == WALL) {
                bool hasPathNeighbors = false;
                if (rx > 0 && rx < rows - 1) {
                    if (grid[rx-1][ry] == PATH && grid[rx+1][ry] == PATH) hasPathNeighbors = true;
                }
                if (ry > 0 && ry < cols - 1) {
                    if (grid[rx][ry-1] == PATH && grid[rx][ry+1] == PATH) hasPathNeighbors = true;
                }
                
                if (hasPathNeighbors) {
                    grid[rx][ry] = PATH;
                }
            }
        }
    }

    // Manhattan distance heuristic for A*
    int heuristic(Point a, Point b) {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }
    
    // A* pathfinding algorithm
    void solveAStar(Point start, Point end, vector<Point>& path, vector<Point>& explored) {
        priority_queue<Node, vector<Node>, greater<Node>> openSet;
        set<Point> closedSet;
        map<Point, Point> cameFrom;
        map<Point, int> gScore;
        
        // Initialize
        Node startNode;
        startNode.pos = start;
        startNode.g = 0;
        startNode.h = heuristic(start, end);
        startNode.f = startNode.h;
        startNode.parent = {-1, -1};
        
        openSet.push(startNode);
        gScore[start] = 0;
        
        while (!openSet.empty()) {
            Node current = openSet.top();
            openSet.pop();
            
            Point currentPos = current.pos;
            
            // Skip if already processed
            if (closedSet.find(currentPos) != closedSet.end()) {
                continue;
            }
            
            // Mark as explored
            explored.push_back(currentPos);
            closedSet.insert(currentPos);
            
            // Check if we reached the goal
            if (currentPos == end) {
                // Reconstruct path
                Point curr = end;
                while (!(curr.x == -1 && curr.y == -1)) {
                    path.push_back(curr);
                    if (cameFrom.find(curr) != cameFrom.end()) {
                        curr = cameFrom[curr];
                    } else {
                        break;
                    }
                }
                reverse(path.begin(), path.end());
                return;
            }
            
            // Check all 4 directions
            int dx[] = {1, 0, -1, 0};
            int dy[] = {0, 1, 0, -1};
            
            for (int i = 0; i < 4; i++) {
                Point neighbor = {currentPos.x + dx[i], currentPos.y + dy[i]};
                
                // Check bounds and if it's a path
                if (neighbor.x < 0 || neighbor.x >= rows || 
                    neighbor.y < 0 || neighbor.y >= cols || 
                    grid[neighbor.x][neighbor.y] == WALL) {
                    continue;
                }
                
                // Skip if already evaluated
                if (closedSet.find(neighbor) != closedSet.end()) {
                    continue;
                }
                
                int tentativeGScore = current.g + 1;
                
                // Check if this path is better
                if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                    cameFrom[neighbor] = currentPos;
                    gScore[neighbor] = tentativeGScore;
                    
                    Node neighborNode;
                    neighborNode.pos = neighbor;
                    neighborNode.g = tentativeGScore;
                    neighborNode.h = heuristic(neighbor, end);
                    neighborNode.f = neighborNode.g + neighborNode.h;
                    neighborNode.parent = currentPos;
                    
                    openSet.push(neighborNode);
                }
            }
        }
    }

    void printJSON() {
        // Generate solution path using A*
        vector<Point> path;
        vector<Point> explored;
        Point start = {0, 1};
        Point end = {rows - 1, cols - 2};
        
        solveAStar(start, end, path, explored);

        // Output JSON for the frontend
        cout << "{";
        cout << "\"rows\":" << rows << ",\"cols\":" << cols << ",";

        // grid
        cout << "\"grid\":[";
        for (int r = 0; r < rows; ++r) {
            cout << "[";
            for (int c = 0; c < cols; ++c) {
                cout << grid[r][c];
                if (c + 1 < cols) cout << ",";
            }
            cout << "]";
            if (r + 1 < rows) cout << ",";
        }
        cout << "],";

        // explored (order matters for animation)
        cout << "\"explored\":[";
        for (size_t i = 0; i < explored.size(); ++i) {
            cout << "{\"x\":" << explored[i].x << ",\"y\":" << explored[i].y << "}";
            if (i + 1 < explored.size()) cout << ",";
        }
        cout << "],";

        // path (shortest path from A*)
        cout << "\"path\":[";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << "{\"x\":" << path[i].x << ",\"y\":" << path[i].y << "}";
            if (i + 1 < path.size()) cout << ",";
        }
        cout << "]";

        cout << "}";
    }
};

int main(int argc, char* argv[]) {
    int size = 15;
    if (argc > 1) {
        size = atoi(argv[1]);
    }
    Maze m(size);
    m.generate();
    m.printJSON();
    return 0;
}