#ifndef GRASS_H
#define GRASS_H
#include "Object.h"

class Grass : public Object {
public:
    static SDL_Surface* image;

    Grass(int x, int y) : Object(GRASS, x, y){}
    ~Grass(){SDL_Log("Grass destr");}
    void onRender(SDL_Renderer *ren);

};

SDL_Surface* Grass::image = SDL_LoadBMP("grass.bmp");

void Grass::onRender(SDL_Renderer *ren){
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, Grass::image);
    SDL_Rect rect;
    rect.x = Object::getX();
    rect.y = Object::getY();
    rect.w = 32;
    rect.h = 32;
    SDL_RenderCopy(ren, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}
#endif // GRASS_H
