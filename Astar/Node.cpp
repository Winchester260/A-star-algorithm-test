#include <vector>
 struct Node {
    int x, y;
    float g;
    float h;
    Node* parent;
    bool setEnd = false;
    bool setBegin = false;

    Node(int x_ = 0, int y_ = 0, Node* p_ = nullptr)
        : x(x_), y(y_), g(FLT_MAX), h(0), parent(p_) {}
};

void setStartNode(Node& start, std::vector<std::vector<int>>& map) {
    for (int x = 0; x < map.size(); x++) {
        for (int y = 0; y < map[0].size(); y++) {

            if (map[x][y] == 3) {
                start = Node(x, y);
                start.setBegin = true;

            }
        }
    }
}
void setEndNode(Node& end, std::vector<std::vector<int>>& map) {
    for (int x = 0; x < map.size(); x++) {
        if (end.setEnd) {
            break;
        }
        for (int y = 0; y < map[0].size(); y++) {

            if (map[x][y] == 2) {
                end = Node(x, y);
                end.setEnd = true;

            }
        }
    }
}
float heuristic(Node* a, Node* b) {
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

bool operator == (const Node& lhs, const Node& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

