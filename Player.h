#ifndef PLAYER_H
#define PLAYER_H
#include "Object.h"
#include "Map.h"

class Player : public Object {
public:
    static SDL_Surface* image;

    Player(int x, int y) : Object(PLAYER, x, y){}
    ~Player(){}
    void onRender(SDL_Renderer *ren);
    void onMove(int x, int y);

};

SDL_Surface* Player::image = SDL_LoadBMP("player.bmp");

void Player::onRender(SDL_Renderer *ren){
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, Player::image);
    SDL_Rect rect;
    rect.x = Object::getX();
    rect.y = Object::getY();
    rect.w = 32;
    rect.h = 32;
    SDL_RenderCopy(ren, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void Player::onMove(int x, int y){
    Object::setX(Object::getX() + x);
    Object::setY(Object::getY() + y);
}
#endif // GRASS_H
