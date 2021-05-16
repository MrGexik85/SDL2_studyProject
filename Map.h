#ifndef MAP_H
#define MAP_H
#include "Grass.h"
#include "Brick.h"
#include "Desert.h"
#include "BoxPlace.h"
#include "Player.h"
#include "Box.h"
#include <fstream>
#include <vector>

class Map {
public:
    std::vector<Object*> static_objects;
    std::vector<Box*> boxes;
    std::vector<Object*> box_places;
    Player *player;

    Map();
    ~Map();
    int loadMapFromFile();
    bool canPlayerMoveLeft();
    bool canPlayerMoveRight();
    bool canPlayerMoveTop();
    bool canPlayerMoveDown();
    bool canBoxMoveLeft(int x, int y);
    bool canBoxMoveRight(int x, int y);
    bool canBoxMoveTop(int x, int y);
    bool canBoxMoveDown(int x, int y);
    void drawMap(SDL_Renderer *ren);
    void repaint(SDL_Renderer *ren, int x, int y);
    void repaintPlayerTopDown(SDL_Renderer *ren);
    void repaintPlayerLeftRight(SDL_Renderer *ren);
};

Map::Map(){
    static_objects.reserve(400);
}

Map::~Map(){
    for(int i = 0; i < static_objects.size(); i++){
        delete static_objects[i];
    }
    for(int i = 0; i < boxes.size(); i++){
        delete boxes[i];
    }
    for(int i = 0; i < box_places.size(); i++){
        delete box_places[i];
    }
    delete player;
}

void Map::repaint(SDL_Renderer *ren, int x, int y){
    for(int i = 0; i < static_objects.size(); i++){
        if(static_objects[i]->getX() == x && static_objects[i]->getY() == y){
            static_objects[i]->onRender(ren);
            SDL_Log("Static object type:%d has been repainted", static_objects[i]->getType());
            break;
        }
    }
    for(int i = 0; i < boxes.size(); i++){
        if(boxes[i]->getX() == x && boxes[i]->getY() == y){
            boxes[i]->onRender(ren);
            SDL_Log("Box has been repainted");
            break;
        }
    }
}

void Map::repaintPlayerTopDown(SDL_Renderer *ren){
    player->onRender(ren);
    repaint(ren, player->getX(), player->getY()+32);
    repaint(ren, player->getX(), player->getY()-32);
    SDL_Log("Player has been repainted");
}

void Map::repaintPlayerLeftRight(SDL_Renderer *ren){
    player->onRender(ren);
    repaint(ren, player->getX()+32, player->getY());
    repaint(ren, player->getX()-32, player->getY());
    SDL_Log("Player has been repainted");
}

void Map::drawMap(SDL_Renderer *ren){
    for(int i = 0; i < static_objects.size(); i++){
        static_objects[i]->onRender(ren);
    }
    for(int i = 0; i < boxes.size(); i++){
        boxes[i]->onRender(ren);
    }
    player->onRender(ren);
}

int Map::loadMapFromFile(){
    std::ifstream in_stream("map.txt");
    static_objects.clear();
    boxes.clear();
    box_places.clear();
    int cur;
    SDL_Log("Loading map...");
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            in_stream >> cur;
            switch(cur){
                case GRASS: {
                    static_objects.push_back(new Grass(j*32, i*32));
                    break;
                }
                case BRICK: {
                    static_objects.push_back(new Brick(j*32, i*32));
                    break;
                }
                case DESERT: {
                    static_objects.push_back(new Desert(j*32, i*32));
                    break;
                }
                case BOX: {
                    boxes.push_back(new Box(j*32, i*32));
                    static_objects.push_back(new Desert(j*32, i*32));
                    break;
                }
                case BOX_PLACE: {
                    box_places.push_back(new BoxPlace(j*32, i*32));
                    static_objects.push_back(new BoxPlace(j*32, i*32));
                    break;
                }
                case PLAYER: {
                    player = new Player(j*32, i*32);
                    static_objects.push_back(new Desert(j*32, i*32));
                    break;
                }
            }
        }
    }
    in_stream.close();
    return 0;
}

bool Map::canPlayerMoveTop(){
    for(int i = 0; i < static_objects.size(); i++){
        if(static_objects[i]->getX() == player->getX() && static_objects[i]->getY() == player->getY()-32){
            SDL_Log("Object type: %d", static_objects[i]->getType());
            if(static_objects[i]->getType() == BRICK){
                return false;
            }
            break;
        }
    }
    for(int i = 0; i < boxes.size(); i++){
        if(boxes[i]->getX() == player->getX() && boxes[i]->getY() == player->getY()-32){
            if(canBoxMoveTop(boxes[i]->getX(), boxes[i]->getY())){
                boxes[i]->onMove(0, -32);
                return true;
            } else {
                return false;
            }
        }
    }

    return true;
}

bool Map::canBoxMoveTop(int x, int y){
    for(int i = 0; i < static_objects.size(); i++){
        if(static_objects[i]->getX() == x && static_objects[i]->getY() == y-32){
            SDL_Log("Object type: %d", static_objects[i]->getType());
            if(static_objects[i]->getType() == BRICK){
                return false;
            }
            break;
        }
    }
    for(int i = 0; i < boxes.size(); i++){
        if(boxes[i]->getX() == x && boxes[i]->getY() == y-32){
            return false;
        }
    }
    return true;
}

bool Map::canPlayerMoveDown(){
    for(int i = 0; i < static_objects.size(); i++){
        if(static_objects[i]->getX() == player->getX() && static_objects[i]->getY() == player->getY()+32){
            SDL_Log("Object type: %d", static_objects[i]->getType());
            if(static_objects[i]->getType() == BRICK){
                return false;
            }
            break;
        }
    }
    for(int i = 0; i < boxes.size(); i++){
        if(boxes[i]->getX() == player->getX() && boxes[i]->getY() == player->getY()+32){
            if(canBoxMoveDown(boxes[i]->getX(), boxes[i]->getY())){
                boxes[i]->onMove(0, 32);
                return true;
            } else {
                return false;
            }
        }
    }
    return true;
}

bool Map::canBoxMoveDown(int x, int y){
    for(int i = 0; i < static_objects.size(); i++){
        if(static_objects[i]->getX() == x && static_objects[i]->getY() == y+32){
            if(static_objects[i]->getType() == BRICK){
                return false;
            }
            break;
        }
    }
    for(int i = 0; i < boxes.size(); i++){
        if(boxes[i]->getX() == x && boxes[i]->getY() == y+32){
            return false;
        }
    }
    return true;
}

bool Map::canPlayerMoveLeft(){
    for(int i = 0; i < static_objects.size(); i++){
        if(static_objects[i]->getX() == player->getX()-32 && static_objects[i]->getY() == player->getY()){
            SDL_Log("Object type: %d", static_objects[i]->getType());
            if(static_objects[i]->getType() == BRICK){
                return false;
            }
            break;
        }
    }
    for(int i = 0; i < boxes.size(); i++){
        if(boxes[i]->getX() == player->getX()-32 && boxes[i]->getY() == player->getY()){
            if(canBoxMoveLeft(boxes[i]->getX(), boxes[i]->getY())){
                boxes[i]->onMove(-32, 0);
                return true;
            } else {
                return false;
            }
        }
    }
    return true;
}

bool Map::canBoxMoveLeft(int x, int y){
    for(int i = 0; i < static_objects.size(); i++){
        if(static_objects[i]->getX() == x-32 && static_objects[i]->getY() == y){
            if(static_objects[i]->getType() == BRICK){
                return false;
            }
            break;
        }
    }
    for(int i = 0; i < boxes.size(); i++){
        if(boxes[i]->getX() == x-32 && boxes[i]->getY() == y){
            return false;
        }
    }
    return true;
}

bool Map::canPlayerMoveRight(){
    for(int i = 0; i < static_objects.size(); i++){
        if(static_objects[i]->getX() == player->getX()+32 && static_objects[i]->getY() == player->getY()){
            SDL_Log("Object type: %d", static_objects[i]->getType());
            if(static_objects[i]->getType() == BRICK){
                return false;
            }
            break;
        }
    }
    for(int i = 0; i < boxes.size(); i++){
        if(boxes[i]->getX() == player->getX()+32 && boxes[i]->getY() == player->getY()){
            if(canBoxMoveRight(boxes[i]->getX(), boxes[i]->getY())){
                boxes[i]->onMove(32, 0);
                return true;
            } else {
                return false;
            }
        }
    }
    return true;
}

bool Map::canBoxMoveRight(int x, int y){
    for(int i = 0; i < static_objects.size(); i++){
        if(static_objects[i]->getX() == x+32 && static_objects[i]->getY() == y){
            SDL_Log("Object type: %d", static_objects[i]->getType());
            if(static_objects[i]->getType() == BRICK){
                return false;
            }
            break;
        }
    }
    for(int i = 0; i < boxes.size(); i++){
        if(boxes[i]->getX() == x+32 && boxes[i]->getY() == y){
            return false;
        }
    }
    return true;
}
#endif // MAP_H
