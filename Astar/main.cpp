#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <time.h>
#include <chrono> // Für std::chrono::milliseconds
#include <thread> // Für std::this_thread::sleep_for
#include <list>
using namespace sf;

Sprite w, r, g, s,astar;

int counter = 0;

struct Node {
    int x, y;
    float g;
    float h;
    Node* parent;

    Node(int x_ = 0, int y_ = 0, Node* p_ = nullptr)
        : x(x_), y(y_), g(FLT_MAX), h(0), parent(p_) {}
};


float heuristic(Node* a, Node* b) {
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

bool operator == (const Node& lhs, const Node& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

void drawMap(RenderWindow* win, int rows, int cols, const std::vector<std::vector<int>>& map) {
    int field = -1;
    const int tileSize = 16; // size of the sprite
    Sprite* currentSprite = nullptr;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            field = map[i][j];

            switch (field)
            {
             case 0:
                   currentSprite = &w;
                   break;
            case 1:
                currentSprite = &r;
                break;
            case 2:
                currentSprite = &g;
                break;
            case 3:
                currentSprite = &s;
                break;
            case 5:
                currentSprite = &astar;
                break;
            default:
                break;
            }

 
            // set the position of the current sprite to the indices of loop and the title size
            currentSprite->setPosition(j * tileSize, i * tileSize);

            //draw the sprite
            win->draw(*currentSprite);
        }
    }

}


int calculatedirectionPlayer(std::vector<std::vector<int>>&map, int x, int y) {
        //0 up , 1 down, 2 left , 3 right
        int playerX = -1, playerY = -1;

        // Finde die Position des Charakters (3) auf der Karte
        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[i].size(); j++) {
                if (map[i][j] == 3) {
                    playerX = i;
                    playerY = j;
                    break;
                }
            }
        }

        // Wenn der Charakter rechts vom Ziel ist und links kein Hindernis liegt
        if (playerX == x && playerY > y && playerY > 0 && map[playerX][playerY - 1] != 1) {
            return 2; // Geh nach links
        }

        // Wenn der Charakter links vom Ziel ist und rechts kein Hindernis liegt
        if (playerX == x && playerY < y && playerY < map[playerX].size() - 1 && map[playerX][playerY + 1] != 1) {
            return 3; // Geh nach rechts
        }

        // Wenn der Charakter über dem Ziel ist und unten kein Hindernis liegt
        if (playerX < x && playerX < map.size() - 1 && map[playerX + 1][playerY] != 1) {
            return 1; // Geh runter
        }

        // Wenn der Charakter unter dem Ziel ist und oben kein Hindernis liegt
        if (playerX > x && playerX > 0 && map[playerX - 1][playerY] != 1) {
            return 0; // Geh hoch
        }

        // Wenn keine der Bedingungen erfüllt ist, gebe eine zufällige Richtung zurück
        return rand() % 4;
    }



void move(int dir,int rows, int cols, std::vector<std::vector<int>>& map) {
    int swap = -1;
    int field = -1;
    int i, j;
    int collected = 0;
    bool placed = false;
    
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            field = map[i][j];
            if (field == 3) {
                break;
            }

        }
        if (field == 3) {
            break;
        }
    }
    //0 up , 1 down, 2 left , 3 right
    switch (dir)
    {
    case 0:
        if (i != 0) { 
            swap = map[i-1][j];
            if (swap == 1) {
                break;
            }
            if (swap == 2) {
                collected = 2;
                swap = 0;
            }
            map[i - 1][j] = 3;
            map[i][j] = swap;
        }
        break;
    case 1:
        if (i < map.size()-1) {
 
            swap = map[i + 1][j];
            if (swap == 1) {
                break;
            }
            if (swap == 2) {
                collected = 2;
                swap = 0;
            }
            map[i + 1][j] = 3;
            map[i][j] = swap;
        }
       
        break;
    case 2:
        if (j != 0) {
 
            swap = map[i][j - 1];
            if (swap == 1) {
                break;
            }
            if (swap == 2) {
                collected = 2;
                swap = 0;
            }
            map[i][j - 1] = 3;
            map[i][j] = swap;
        }
        
        break;
    case 3:
        if (j < map[0].size()-1) {
 
            swap = map[i][j+1];
            if (swap == 1) {
                break;
            }
            if (swap == 2) {
                collected = 2;
                swap = 0;
            }
            map[i][j+1] = 3;
            map[i][j] = swap;
        }
        break;
    default:
        break;
    }
    if (collected == 2) {
 
        while (placed == false) {
        
         
        int randomI = rand() % map.size();
        int randomJ = rand() % map[0].size();
 
            if (map[randomI][randomJ] != 1) {
                placed = true;
                counter++;
                try {
                    map[randomI][randomJ] = 2;
                }
                catch (const std::out_of_range& e) {
                    std::cerr << "Fehler: " << e.what() << std::endl;

                }
                collected = 0;
            }
        }
 
    }
}

std::list<Node> aStar(const std::vector<std::vector<int>>& map, Node start, Node end) {

    try {

        std::list<Node> openList, closedList;

        start.g = 0;
        start.h = heuristic(&start, &end);
        openList.push_back(start);

        while (!openList.empty()) {
            openList.sort([](const Node& a, const Node& b) { return a.g + a.h < b.g + b.h; });

            Node current = openList.front();
            openList.pop_front();

            if (current == end) {
                std::list<Node> path;
                while (current.parent) {
                    path.push_back(current);
                    current = *current.parent;
                }
                path.push_back(start);
                return path;
            }

            closedList.push_back(current);
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (x == 0 && y == 0)
                        continue;

                    int newX = current.x + x;
                    int newY = current.y + y;

                    if (newX < 0 || newX >= map.size() || newY < 0 || newY >= map[0].size() || map[newX][newY] == 1)
                        continue;

                    Node successor(newX, newY, &closedList.back());

                    if (std::find(closedList.begin(), closedList.end(), successor) != closedList.end())
                        continue;

                    float tentative_g = current.g + std::sqrt(x * x + y * y);

                    if (std::find(openList.begin(), openList.end(), successor) == openList.end() || tentative_g < successor.g) {
                        successor.g = tentative_g;
                        successor.h = heuristic(&successor, &end);
                        successor.parent = &closedList.back();

                        if (std::find(openList.begin(), openList.end(), successor) == openList.end()) {
                            openList.push_back(successor);
                        }
                    }
                }
            }


        }

        return std::list<Node>();  // Kein Pfad gefunden


    }
    catch (const std::out_of_range& e) {
        std::cerr << "Fehler: " << e.what() << std::endl;

    }
     
}

void drawNewMap(std::vector<std::vector<int>> & map, int x, int y) {
    try {
        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[0].size(); j++) {
                if (i == x && j == y)
                {
                    map[i][j] = 5;
                }
            }
        }
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Fehler: " << e.what() << std::endl;

    }
}

void deletePath(std::vector<std::vector<int>>& map) {

 
        for (int i = 0; i < map.size() ; i++) {
            for (int j = 0; j < map[0].size() ; j++) {
                if (map[i][j] == 5) {
                    map[i][j] = 0;
                }
             }
        }
  
 
 
}
 

int main() {
    
    int rows = 18;
    int column = 37;
    int height = 16 * rows;
    int width = 16 * column;
    int dir = -1;
    int fps = 0;
    srand(time(NULL));

    //2D Map
    std::vector<std::vector<int>> map = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0},
        {0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };


	RenderWindow window(VideoMode(width, height), "A-star Algorithm", Style::Titlebar | Style::Close);
    
    //Textures
    Texture white, red, goal,start,road;
    white.loadFromFile("images/white.png");
    red.loadFromFile("images/red.png");
    goal.loadFromFile("images/fleisch.jpg");
    start.loadFromFile("images/luffy.jpg");
    road.loadFromFile("images/green.png");
    //Sprites
    w.setTexture(white);
    r.setTexture(red);
    g.setTexture(goal);
    s.setTexture(start);
    astar.setTexture(road);
    
 
    Node begin, end;


    // Window Loop
    while (window.isOpen())
    {
         try {
            for (int x = 0; x < map.size(); x++) {
                for (int y = 0; y < map[0].size(); y++) {
                    if (map[x][y] == 3) {
                        begin = Node(x, y);

                    }
                    if (map[x][y] == 2) {
                        end = Node(x, y);

                    }
                }
            }

            std::list<Node> path = aStar(map, begin, end);
            
            if (path.size() > 1 && fps % 600 == 1) {
                Node n = *std::prev(path.end(), 2);
                dir = calculatedirectionPlayer(map, n.x, n.y);
 
            }
            for (const auto& node : path) {

                if (node.x == begin.x && node.y == begin.y) {
                    continue;
                }
                if (node.x == end.x && node.y == end.y) {

                }
                else {
 
                    drawNewMap(map, node.x, node.y);
                }
            }
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Fehler: " << e.what() << std::endl;

        }
        

        window.setTitle("A - Star Algorithm - Counter: " + std::to_string(counter));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Tastaturereignisse abfangen
            if (event.type == sf::Event::KeyPressed)
            { 
                deletePath(map);
                switch (event.key.code)
                {
                    
                case sf::Keyboard::W:
                    dir = 0;
                    break;

                case sf::Keyboard::A:
                    dir = 2;
                    break;

                case sf::Keyboard::S:
                    dir = 1;
                    break;

                case sf::Keyboard::D:
                    dir = 3;
                    break;

                default:
                    break;
                }
            }
        }
         
         
         
        if (fps % 600 == 1) {
             
            move(dir, map.size(), map[0].size(), map);
            fps = 0;
            
            dir = -1;
             
            
        }
        window.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
         
        drawMap(&window, map.size() , map[0].size(), map);
        window.display();
        deletePath(map);
        fps++;
    }

	return 0;
}