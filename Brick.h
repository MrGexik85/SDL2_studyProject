#ifndef BRICK_H
#define BRICK_H
#include "Object.h"

class Brick : public Object {
public:
    static SDL_Surface* image;

    Brick(int x, int y) : Object(BRICK, x, y){}
    ~Brick(){SDL_Log("Brick destr");}
    void onRender(SDL_Renderer *ren);

};

SDL_Surface* Brick::image = SDL_LoadBMP("brick.bmp");

void Brick::onRender(SDL_Renderer *ren){
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, Brick::image);
    SDL_Rect rect;
    rect.x = Object::getX();
    rect.y = Object::getY();
    rect.w = 32;
    rect.h = 32;
    SDL_RenderCopy(ren, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}
#endif // GRASS_H
