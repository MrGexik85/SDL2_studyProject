#ifndef BOX_H
#define BOX_H
#include "Object.h"

class Box : public Object {
public:
    static SDL_Surface* image;

    Box(int x, int y) : Object(BOX, x, y){}
    ~Box(){}
    void onRender(SDL_Renderer *ren);
    void onMove(int x, int y);

};

SDL_Surface* Box::image = SDL_LoadBMP("box.bmp");

void Box::onRender(SDL_Renderer *ren){
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, Box::image);
    SDL_Rect rect;
    rect.x = Object::getX();
    rect.y = Object::getY();
    rect.w = 32;
    rect.h = 32;
    SDL_RenderCopy(ren, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void Box::onMove(int x, int y){
    Object::setX(Object::getX() + x);
    Object::setY(Object::getY() + y);
}
#endif // GRASS_H
