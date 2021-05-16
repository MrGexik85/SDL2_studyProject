#ifndef DESERT_H
#define DESERT_H
#include "Object.h"

class Desert : public Object {
public:
    static SDL_Surface* image;

    Desert(int x, int y) : Object(DESERT, x, y){}
    ~Desert(){SDL_Log("Desert destr");}
    void onRender(SDL_Renderer *ren);

};

SDL_Surface* Desert::image = SDL_LoadBMP("desert.bmp");

void Desert::onRender(SDL_Renderer *ren){
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, Desert::image);
    SDL_Rect rect;
    rect.x = Object::getX();
    rect.y = Object::getY();
    rect.w = 32;
    rect.h = 32;
    SDL_RenderCopy(ren, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}
#endif // GRASS_H
