#pragma once
#include <vector>
struct Node {
 
    int gCost;
    int fCost;
    int col;
    int row;
    int  hCost;
    Node *parent;
    bool setEnd = false;
    bool setBegin = false;
    bool setChecked = false;
    bool setAsOpen = false;
    bool setBarrier = false;

    Node(int col_ = 0, int row_ = 0, Node* p_ = nullptr)
        : col(col_), row(row_), gCost(FLT_MAX), hCost(0), parent(p_) {}
};

bool setStartNode(Node& start, std::vector<std::vector<int>>& map, Node &currentNode) {
    for (int x = 0; x < map.size(); x++) {
        for (int y = 0; y < map[0].size(); y++) {

            if (map[x][y] == 3) {
                start = Node(x, y);
                start.setBegin = true;
                currentNode = start;
                return true;
            }
        }
    }
}

void setAsChecked(Node &n) {
    n.setChecked = true;
}
bool setEndNode(Node& end, std::vector<std::vector<int>>& map) {
    for (int x = 0; x < map.size(); x++) {
        if (end.setEnd) {
            break;
        }
        for (int y = 0; y < map[0].size(); y++) {

            if (map[x][y] == 2) {
                end = Node(x, y);
                end.setEnd = true;
                return true;

            }
        }
    }
}

bool operator == (const Node& lhs, const Node& rhs) {
    return lhs.col == rhs.col && lhs.row == rhs.row;
}
 

