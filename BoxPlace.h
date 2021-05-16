#ifndef BPLACE_H
#define BPLACE_H
#include "Object.h"

class BoxPlace : public Object {
public:
    static SDL_Surface* image;

    BoxPlace(int x, int y) : Object(BOX_PLACE, x, y){}
    ~BoxPlace(){}
    void onRender(SDL_Renderer *ren);

};

SDL_Surface* BoxPlace::image = SDL_LoadBMP("box_place.bmp");

void BoxPlace::onRender(SDL_Renderer *ren){
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, BoxPlace::image);
    SDL_Rect rect;
    rect.x = Object::getX();
    rect.y = Object::getY();
    rect.w = 32;
    rect.h = 32;
    SDL_RenderCopy(ren, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}
#endif // GRASS_H
