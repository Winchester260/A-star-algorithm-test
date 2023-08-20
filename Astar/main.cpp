#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <chrono> // F�r std::chrono::milliseconds
#include <thread> // F�r std::this_thread::sleep_for
#include <list>
#include "node.hpp"
#include "AstarAlgorithmn.h"
using namespace sf;

Sprite w, r, g, s,astar, wallSprite, treeSprite;
int counter = 0;
bool newRound = false;
int OriginaltileSize = 16; // size of the sprite
 
enum tilesType{grass, water, meat,player,tree,aPath,wall };
 
void drawMap(RenderWindow* win, int rows, int cols, const std::vector<std::vector<int>>& map) {
    int field = -1;
     
    Sprite* currentSprite = nullptr;
     
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            field = map[i][j];

            switch (field)
            {
            case 0: case 5:

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
            case 4:
                currentSprite = &treeSprite;
                break;
 
            case 6:
                currentSprite = &wallSprite;
                break;
            default:
                break;
            }

            currentSprite->setScale(3, 3);
            // set the position of the current sprite to the indices of loop and the title size
            currentSprite->setPosition(j * OriginaltileSize, i * OriginaltileSize);

            //draw the sprite
            win->draw(*currentSprite);
        }
    }

}

 


void move(int dir,int rows, int cols, std::vector<std::vector<int>>& map) {
    int swap = -1;
    int field = -1;
    int i= 0, j=0;
    int collected = 0;
    bool placed = false;
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            field = map[i][j];
            if (field == tilesType(3)) {
                break;
            }

        }
        if (field == tilesType(3)) {
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
            if (swap == 4) {
                break;
            }
            if (swap == 2) {
                collected = 2;
                swap = 0;
            }
            if (swap == 5) {
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
            if (swap == 4) {
                break;
            }
            if (swap == 2) {
                collected = 2;
                swap = 0;
            }
            if (swap == 5) {
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
            if (swap == 4) {
                break;
            }
            if (swap == 2) {
                collected = 2;
                swap = 0;
            }
            if (swap == 5) {
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
            if (swap == 4) {
                break;
            }
            if (swap == 2) {
                collected = 2;
                swap = 0;
            }
            if (swap == 5) {
                swap = 0;
            }
            map[i][j+1] = 3;
            map[i][j] = swap;
        }
        break;
    default:
        break;
    }

    //update position of running guy
    startx = i;
    starty = j;
    if (collected == 2) {
 
        while (placed == false) {
        
         
        int randomI = rand() % map.size();
        int randomJ = rand() % map[0].size();
 
            if (map[randomI][randomJ] != 1 && map[randomI][randomJ] != 4 && map[randomI][randomJ] != 6 &&  map[randomI][randomJ] != 3) {
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

int getDirection(std::vector<std::vector<int>>& map) {

    // Finde die Position des Charakters (3) auf der Karte
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == 3) {
                
                if (j > 0 && map[i][j-1] == 5 || j > 0 && map[i][j - 1] == 2) {
                  //  std::cout << " direction left: ";
                    return 2;
                }
                if (j < map[i].size() - 1 && map[i][j+1] == 5 || j < map[i].size() - 1 && map[i][j + 1] == 2) {
                   // std::cout << " direction right: ";
                    return 3;
                }

 
                if (i > 0 && map[i-1][j] == 5 || i > 0 && map[i - 1][j] == 2) {
                  //  std::cout << " direction up: ";
                    return 0;
                }
                if (i < map.size()-1&&( map[i + 1][j] == 5) || i < map.size() - 1 && map[i + 1][j] == 2) {
                  //  std::cout << " direction down: ";
                    return 1;
                }
 
            }
        }
    }

    return -1;
}

void drawNewMap(std::vector<std::vector<int>> & map, int x, int y) {
 
        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[0].size(); j++) {
                if (i == x && j == y)
                {
                    map[i][j] = 5;
                }
            }
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
    int scale = 3;
    OriginaltileSize = OriginaltileSize * scale;
    const int rows = 12;
    const int column = 16;
    int height = OriginaltileSize * rows;
    int width = OriginaltileSize * column;
    int dir = -1;
    int fps = 0;
     
    srand(time(NULL));
    RenderWindow window(VideoMode(width, height), "A-star Algorithm", Style::Titlebar | Style::Close);
    sf::Event event;
     

    //Textures
    Texture white, red, goal, start, road, wall,tree;
    tree.loadFromFile("images/tree.png");
    wall.loadFromFile("images/wall.png");
    white.loadFromFile("images/grass.png");
    red.loadFromFile("images/water.png");
    goal.loadFromFile("images/fleisch.jpg");
    start.loadFromFile("images/l4.png");
    road.loadFromFile("images/green.png");
    //Sprites
    wallSprite.setTexture(wall);
    w.setTexture(white);
    r.setTexture(red);
    g.setTexture(goal);
    s.setTexture(start);
 
    treeSprite.setTexture(tree);
 
 
 

    //2D Map 16x12
    std::vector<std::vector<int>> map2 = {
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,0,1,1,1,1,1,0,0,0,1,1,0,0,0,6},
        {6,0,1,1,0,1,1,0,0,0,1,1,0,1,1,6},
        {6,0,1,0,0,0,1,0,0,0,0,3,0,1,1,6},
        {6,0,1,0,0,0,1,0,0,0,0,0,0,0,0,6},
        {6,0,1,0,0,0,1,0,0,0,0,0,0,0,0,6},
        {6,2,0,0,0,0,0,0,0,0,0,0,0,4,4,6},
        {6,0,1,0,0,0,1,0,0,0,0,0,0,4,4,6},
        {6,0,1,0,0,0,1,0,0,0,0,0,0,0,0,6},
        {6,0,1,1,0,1,1,0,0,0,0,0,1,1,0,6},
        {6,0,1,1,0,1,1,0,0,0,0,0,1,1,0,6},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    };
 
    //2D Map
    std::vector<std::vector<int>> map1 = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,2,1,0,0,1,1,1,1,1,1,0,1,0,0,0,0},
        {0,1,0,0,0,0,0,0,1,1,1,1,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0},
        {0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,0,0,0},
        {0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,1,0,1,0,1,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,0,1,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0},
        {1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,0,0,0,0},
        {3,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

 
    std::vector<std::vector<Node>> mapNodes(map2.size(), std::vector<Node>(map2[0].size()));

 
 
    initializeMapAndNodes(map2, mapNodes);
    setCostOnNodes(map2, mapNodes, begin, *end);
 
 

    // Window Loop
    while (window.isOpen())
    {
        window.setTitle("A - Star Algorithm - Counter: " + std::to_string(counter));
 
        //if running guy is on position of goal, then check for a new round
         if (goalx == startx && goaly == starty) {
             newRound = true;
        }       
        
        if (newRound) {
            newRound = false;
            initializeMapAndNodes(map2,mapNodes);
            setCostOnNodes(map2, mapNodes, begin, *end);
            aStar(map2, mapNodes, begin, *end);
        }
 

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Tastaturereignisse abfangen
            if (event.type == sf::Event::KeyPressed)
            { 
 
                switch (event.key.code)
                {
                 
 
                case sf::Keyboard::W:
                    newRound = false;
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
                    aStar(map2, mapNodes, begin, *end); 
                    break;
                }

                 
            }
             
        }
         
         
         
        if (fps % 600 == 1) {             
            move(dir, map2.size(), map2[0].size(), map2);
            fps = 0;
            dir = getDirection(map2);
        }
 
        window.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        drawMap(&window, map2.size() , map2[0].size(), map2);
 
        window.display(); 
        fps++;
    }
    
	return 0;
}