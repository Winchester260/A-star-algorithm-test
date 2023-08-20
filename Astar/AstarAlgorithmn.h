#pragma once

std::vector<Node*> openList;
std::vector<Node> closedList;
bool goalreached = false;
int goalx;
int goaly;
int startx;
int starty;
Node begin, * end, * current;

void getCost(Node& n, Node& startNode, Node& goalNode) {
    //get g cost
    int xDistance = abs(n.row - startNode.row);
    int yDistance = abs(n.col - startNode.col);
    n.gCost = xDistance + yDistance;
    //get h cost
    xDistance = abs(n.row - goalNode.row);
    yDistance = abs(n.col - goalNode.col);
    n.hCost = xDistance + yDistance;

    //get f cost
    n.fCost = n.hCost + n.gCost;

}
void setCostOnNodes(const std::vector<std::vector<int>>& map, std::vector<std::vector<Node>>& mapNodes, Node& start, Node& end) {

    int col = 0;
    int row = 0;

    while (col < map.size() && row < map[0].size()) {
        getCost(mapNodes[col][row], start, end);


        col++;
        if (col == map.size()) {
            col = 0;
            row++;
        }
    }
}
bool openNode(Node* node) {

    if (!node->setChecked && !node->setAsOpen && !node->setBegin && !node->setBarrier) {
        node->setAsOpen = true;

        node->parent = current;
        openList.push_back(node);
        return true;
    }
    return false;
}
void trackThePath(std::vector<std::vector<int>>& map, Node& cur, Node& start) {
    Node* currentnode = &cur;


    while (!(*currentnode == start)) {
        currentnode = currentnode->parent;


        if (!(*currentnode == start)) {
            if (!(currentnode == end)) {
                map[currentnode->col][currentnode->row] = 5;
            }

        }
    }
}
void aStar(std::vector<std::vector<int>>& map, std::vector<std::vector<Node>>& mapNodes, Node& start, Node& end) {

    while (!goalreached) {
        int col = current->col;
        bool open1 = false;
        bool open2 = false;
        bool open3 = false;
        bool open4 = false;
        int row = current->row;
        setAsChecked(*current);
        closedList.push_back(*current);
        openList.erase(std::remove(openList.begin(), openList.end(), current), openList.end());

        try {
            if (row - 1 >= 0) {
                open1 = openNode(&mapNodes.at(col).at(row - 1));
            }
            if (col - 1 >= 0) {

                open2 = openNode(&mapNodes.at(col - 1).at(row));
            }
            if (row + 1 < map[0].size()) {

                open3 = openNode(&mapNodes.at(col).at(row + 1));
            }
            if (col + 1 < map.size()) {

                open4 = openNode(&mapNodes.at(col + 1).at(row));
            }

        }
        catch (const std::exception& e) {
            std::cout << "error occured: " << e.what() << std::endl;
            std::cout << "error occured: open1: " << open1 << std::endl;
            std::cout << "error occured: open2: " << open2 << std::endl;
            std::cout << "error occured: open3: " << open3 << std::endl;
            std::cout << "error occured: open4: " << open4 << std::endl;
        }

        int bestNodeIndex = -1;
        int bestNodefCost = 999;

        for (int i = 0; i < openList.size(); i++) 
        {
            if (openList[i]->fCost < bestNodefCost) {
                bestNodeIndex = i;
                bestNodefCost = openList[i]->fCost;
            }
            else if (openList[i]->fCost == bestNodefCost) {
                if (openList[i]->gCost < openList[bestNodeIndex]->gCost) {
                    bestNodeIndex = i;
                }
            }
        }
        if (bestNodeIndex != -1) {
            current = openList[bestNodeIndex];


        }

        if (current->col == end.col && current->row == end.row) {
            goalreached = true;
            trackThePath(map, *current, start);



        }

    }



}

void initializeMapAndNodes(std::vector<std::vector<int>>& map, std::vector<std::vector<Node>>& mapNodes) {
    goalreached = false;
    openList.clear();
    closedList.clear();
    int col = 0;
    int row = 0;
    while (col < map.size() && row < map[0].size()) {
        mapNodes[col][row] = Node(col, row);

        if (map[col][row] == 2) {

            end = &mapNodes[col][row];
            end->setEnd = true;
            mapNodes[col][row].setEnd = true;
            goalx = end->col;
            goaly = end->row;

        }
        if (map[col][row] == 1 || map[col][row] == 4 || map[col][row] == 6) {
            mapNodes[col][row].setBarrier = true;
        }
        if (map[col][row] == 3) {
            mapNodes[col][row].setBegin = true;
            current = &mapNodes[col][row];
            begin = mapNodes[col][row];
            starty = begin.row;
            startx = begin.col;
        }

        col++;
        if (col == map.size()) {
            col = 0;
            row++;
        }
    }
}