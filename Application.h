#ifndef APPLICATION_H
#define APPLICATION_H

#include <sstream>
#include "SDL_image.h"
#include "SDL.h"
#include "Map.h"
#include "Menu.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

enum state{MAIN_MENU = 0, GAME, PAUSE, GAME_END, GAME_PAUSE, RULES, TOP_LIST, RESTART};

class Application
{
    public:
        Application();
        ~Application();
        int Exec();

    private:
        bool Init();
        void Clean();
        void CheckGameContext();
        void Render();
        void ExitEvent();
        void KeyDownEvent(SDL_Keycode keyCode, Uint16 mod, SDL_Scancode scancode);
        void drawScore();
        void OnEvent(SDL_Event *event);

        SDL_Window *win;
        SDL_Renderer *ren;
        TTF_Font *font;
        Map *gameMap;
        Menu *menu;
        bool Running;
        int Steps;
        char State;
};

Application::Application()
{
    gameMap = new Map();
    menu = new Menu();
    Running = true;
    State = MAIN_MENU;
    Steps = 0;
}

Application::~Application(){}

void Application::drawScore(){
    std::ostringstream oss;
    oss << "STEPS: " << Steps;
    std::string score(oss.str());

    SDL_Color color = {0, 0, 0};
    SDL_Surface *surf = TTF_RenderText_Blended(font, score.c_str(), color);
    if(surf == NULL){
        SDL_Log("Error in drawScore create surface");
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect rect = {20, 35, surf->w, surf->h};
    for(int i = 0; i < 5; i++){
        gameMap->repaint(ren, i*32, 32);
    }
    SDL_RenderCopy(ren, texture, NULL, &rect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(texture);
    SDL_Log("Score has been paint");
}

bool Application::Init(){
    if(TTF_Init() == -1){
        return false;
    }
    win = SDL_CreateWindow("Sokoban", 100, 100, 640, 640, SDL_WINDOW_ALLOW_HIGHDPI);

    if (!win) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to create window. See the log for more info.", NULL);
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window, error: %s", SDL_GetError());
        return false;
    }
    SDL_Log("Window create - success");

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if(!ren){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to create renderer. See the log for more info.", NULL);
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Unable to create renderer, error: %s", SDL_GetError());
        return false;
    }
    SDL_Log("Creating renderer - succeeded");
    font = TTF_OpenFont("times.ttf", 24);
    if(font == NULL){
        SDL_Log("Error in drawScore open font");
        SDL_Log("%s", SDL_GetError());
        return false;
    }

    gameMap->loadMapFromFile();
    menu->drawMainMenu(ren);
    SDL_RenderPresent(this->ren);

    return true;
}

int Application::Exec(){
    if(Init() == false){
        return -1;
    }

    SDL_Event event;

    while(Running){
        if(SDL_WaitEvent(&event)){
            OnEvent(&event);
        }
        Render();
    }

    Clean();

    return 0;
}

void Application::CheckGameContext(){
    for(int i = 0; i < gameMap->box_places.size(); i++){
        bool onPlace = false;
        for(int j = 0; j < gameMap->boxes.size(); j++){
            if(gameMap->boxes[j]->getX() == gameMap->box_places[i]->getX() && gameMap->boxes[j]->getY() == gameMap->box_places[i]->getY()){
                onPlace = true;
            }
        }
        if(!onPlace) {
            SDL_Log("Continue game");
            return;
        }
    }
    SDL_Log("Game end");
    menu->resetPoint(GAME_END);
    menu->drawGameEnd(ren, Steps);
    SDL_RenderPresent(ren);
    State = GAME_END;
}

void Application::Render(){
    return;
}

void Application::Clean(){
    delete gameMap;
    delete menu;
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_CloseFont(font);
    delete Box::image;
    delete BoxPlace::image;
    delete Brick::image;
    delete Desert::image;
    delete Grass::image;
    delete Player::image;
}

void Application::ExitEvent(){
    Running = false;
}

void Application::KeyDownEvent(SDL_Keycode keyCode, Uint16 mod, SDL_Scancode scancode){
    switch(keyCode){
        case SDLK_UP:{
            if(State == GAME){
                if(gameMap->canPlayerMoveTop()){
                    gameMap->player->onMove(0, -32);
                    gameMap->repaintPlayerTopDown(ren);
                    Steps++;
                    drawScore();
                    SDL_RenderPresent(ren);
                    CheckGameContext();
                }
            } else {
                menu->eventTop(State, ren, Steps);
                SDL_RenderPresent(ren);
            }
            break;
        }

        case SDLK_DOWN:{
            if(State == GAME){
                if(gameMap->canPlayerMoveDown()){
                    gameMap->player->onMove(0, 32);
                    gameMap->repaintPlayerTopDown(ren);
                    Steps++;
                    drawScore();
                    SDL_RenderPresent(ren);
                    CheckGameContext();
                }
            } else {
                menu->eventDown(State, ren, Steps);
                SDL_RenderPresent(ren);
            }
            break;
        }

        case SDLK_LEFT:{
            if(State == GAME){
                if(gameMap->canPlayerMoveLeft()){
                    gameMap->player->onMove(-32, 0);
                    gameMap->repaintPlayerLeftRight(ren);
                    Steps++;
                    drawScore();
                    SDL_RenderPresent(ren);
                    CheckGameContext();
                }
            }
            break;

        }

        case SDLK_RIGHT:{
            if(State == GAME){
                if(gameMap->canPlayerMoveRight()){
                    gameMap->player->onMove(32, 0);
                    gameMap->repaintPlayerLeftRight(ren);
                    Steps++;
                    drawScore();
                    SDL_RenderPresent(ren);
                    CheckGameContext();
                }
            }
            break;
        }

        case SDLK_ESCAPE:{
            SDL_Log("Key Escape");
            if(State == GAME){
                menu->resetPoint(PAUSE);
                menu->drawPause(ren);
                State = PAUSE;
                SDL_RenderPresent(ren);
            } else if (State == RULES || State == TOP_LIST) {
                menu->resetPoint(MAIN_MENU);
                menu->drawMainMenu(ren);
                State = MAIN_MENU;
                SDL_RenderPresent(ren);
            }
            break;
        }

        case SDLK_RETURN: {
            SDL_Log("Key Enter");
            if(State != GAME){
                State = menu->enterEvent(State, ren);
                if(State == GAME) {
                    gameMap->drawMap(ren);
                    drawScore();
                } else if(State == RESTART){
                    Steps = 0;
                    gameMap->loadMapFromFile();
                    gameMap->drawMap(ren);
                    drawScore();
                    State = GAME;
                }
                SDL_RenderPresent(ren);
            }
            break;
        }
    }
}

void Application::OnEvent(SDL_Event *event){
    switch(event->type){
        case SDL_QUIT: {
            SDL_Log("SDL_QUIT event");
            ExitEvent();
            break;
        }
        case SDL_KEYDOWN:{
            SDL_Log("KeyDown event");
            KeyDownEvent(event->key.keysym.sym, event->key.keysym.mod, event->key.keysym.scancode);
            break;
        }
    }
}

#endif // APPLICATION_H
