#ifndef OBJECT_H
#define OBJECT_H
#include "SDL.h"

enum type{OBJECT = 0, GRASS, BRICK, DESERT, BOX, BOX_PLACE, PLAYER};

class Object {
    private:
        int m_type;
        int m_x;
        int m_y;
    public:
        Object(int t, int x, int y) : m_type(t), m_x(x), m_y(y){}
        virtual ~Object(){}
        int getType(){return m_type;}
        int getX(){return m_x;}
        int getY(){return m_y;}
        void setX(int x){m_x = x;}
        void setY(int y){m_y = y;}
        virtual void onRender(SDL_Renderer* ren){}
};

#endif // OBJECT_H
