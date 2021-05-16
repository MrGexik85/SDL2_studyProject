#ifndef MENU_H
#define MENU_H

#include <fstream>
#include <sstream>
#include "SDL_ttf.h"

enum menu_state{MAIN_MENU_ = 0, GAME_, PAUSE_, GAME_END_, GAME_PAUSE_, RULES_, TOP_LIST_, RESTART_};

class Menu {
public:
    SDL_Rect point;

    Menu();
    ~Menu(){}
    void drawMainMenu(SDL_Renderer *ren);
    void drawPause(SDL_Renderer *ren);
    void drawGameEnd(SDL_Renderer *ren, int score);
    void drawRules(SDL_Renderer *ren);
    void drawTopList(SDL_Renderer *ren);
    char enterEvent(char state, SDL_Renderer *ren);
    void resetPoint(char state);
    void eventTop(char state, SDL_Renderer *ren, int score);
    void eventDown(char state, SDL_Renderer *ren, int score);
};

Menu::Menu(){
    point = {220, 282, 10, 10};
}



void Menu::resetPoint(char state){
    switch (state){
        case MAIN_MENU_ : {
            point.y = 282;
            break;
        }
        case PAUSE_ : {
            point.y = 275;
            break;
        }
        case GAME_END_ : {
            point.y = 378;
            break;
        }
    }
}

char Menu::enterEvent(char state, SDL_Renderer *ren){
    switch (state){
        case MAIN_MENU_:{
            if(point.y == 282){
                return RESTART_;
            } else if(point.y == 336){
                drawTopList(ren);
                return TOP_LIST_;
            } else if(point.y == 390){
                drawRules(ren);
                return RULES_;
            } else if(point.y == 444){
                SDL_Event exit;
                exit.type = SDL_QUIT;
                SDL_PushEvent(&exit);
                return state;
            }
            break;
        }
        case PAUSE_: {
            if(point.y == 275){
                return GAME_;
            } else if(point.y == 333){
                return RESTART_;
            } else if(point.y == 391){
                resetPoint(MAIN_MENU_);
                drawMainMenu(ren);
                return MAIN_MENU_;
            }
            break;
        }
        case GAME_END_: {
            if(point.y == 378){
                return RESTART_;
            } else if(point.y == 435){
                resetPoint(MAIN_MENU_);
                drawMainMenu(ren);
                return MAIN_MENU_;
            }
            break;
        }
    }
}

void Menu::eventTop(char state, SDL_Renderer *ren, int score){
    switch(state){
        case MAIN_MENU_ : {
            if(point.y > 282) {
                point.y -= 54;
            }
            drawMainMenu(ren);
            break;
        }
        case PAUSE_ : {
            if(point.y > 275){
                point.y -= 58;
            }
            drawPause(ren);
            break;
        }
        case GAME_END_ : {
            if(point.y > 378){
                point.y -= 57;
            }
            drawGameEnd(ren, score);
            break;
        }
    }
}

void Menu::eventDown(char state, SDL_Renderer *ren, int score){
    switch(state){
        case MAIN_MENU_ : {
            if(point.y < 444) {
                point.y += 54;
            }
            drawMainMenu(ren);
            break;
        }
        case PAUSE_ : {
            if(point.y < 391){
                point.y += 58;
            }
            drawPause(ren);
            break;
        }
        case GAME_END_ : {
            if(point.y < 435){
                point.y += 57;
            }
            drawGameEnd(ren, score);
            break;
        }
    }
}

void Menu::drawGameEnd(SDL_Renderer *ren, int score){
    SDL_Surface *surf = SDL_LoadBMP("GameEnd.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
    SDL_RenderCopy(ren, texture, NULL, NULL);
    TTF_Font *font = TTF_OpenFont("times.ttf", 18);
    int top[3];
    bool new_record = false;
    std::ifstream in("records.txt");
    in >> top[0] >> top[1] >> top[2];
    in.close();
    for(int i = 0; i < 3; i++){
        if (score < top[i]){
            for(int j = i+1; j < 2; j++){
                top[j] = top[j-1];
            }
            top[i] = score;
            new_record = true;
            break;
        }
    }
    std::ofstream out("records.txt");
    for(int i = 0; i < 3; i++){
        out << top[i] << "\n";
    }
    out.close();
    std::ostringstream oss;
    oss << "Your steps: " << score;
    std::string gamescore(oss.str());
    SDL_Surface *surf_text = TTF_RenderText_Blended(font, gamescore.c_str(), {0, 0, 0});
    SDL_Texture *texture_text = SDL_CreateTextureFromSurface(ren, surf_text);
    SDL_Rect rect = {280, 260, surf_text->w, surf_text->h};
    SDL_RenderCopy(ren, texture_text, NULL, &rect);
    SDL_DestroyTexture(texture_text);
    SDL_FreeSurface(surf_text);
    if(new_record){
        surf_text = TTF_RenderText_Blended(font, "NEW RECORD", {255, 100, 100});
        texture_text = SDL_CreateTextureFromSurface(ren, surf_text);
        SDL_Rect rect = {280, 280, surf_text->w, surf_text->h};
        SDL_RenderCopy(ren, texture_text, NULL, &rect);
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(texture_text);
    SDL_FreeSurface(surf_text);
    TTF_CloseFont(font);
    SDL_RenderFillRect(ren, &point);
}

void Menu::drawRules(SDL_Renderer *ren){
    SDL_Surface *surf = SDL_LoadBMP("Rules.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
    SDL_RenderCopy(ren, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surf);
}

void Menu::drawTopList(SDL_Renderer *ren){
    SDL_Surface *surf = SDL_LoadBMP("TopList.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
    TTF_Font *font = TTF_OpenFont("times.ttf", 18);
    int top[3];
    std::ifstream in("records.txt");
    std::ostringstream oss;
    in >> top[0] >> top[1] >> top[2];
    oss << "1: " << top[0] << "\n2: " << top[1] << "\n3: " << top[2];
    std::string str(oss.str());
    SDL_Surface *surf_text = TTF_RenderText_Blended_Wrapped(font, str.c_str(), {0, 0, 0}, 55);
    SDL_Texture *texture_text = SDL_CreateTextureFromSurface(ren, surf_text);
    SDL_Rect rect = {300, 270, surf_text->w, surf_text->h};
    SDL_RenderCopy(ren, texture, NULL, NULL);
    SDL_RenderCopy(ren, texture_text, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surf);
    in.close();
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture_text);
    SDL_FreeSurface(surf_text);
}

void Menu::drawMainMenu(SDL_Renderer *ren){
    SDL_Surface *surf = SDL_LoadBMP("MainMenu.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
    SDL_RenderCopy(ren, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surf);
    SDL_RenderFillRect(ren, &point);
}

void Menu::drawPause(SDL_Renderer *ren){
    SDL_Surface *surf = SDL_LoadBMP("Pause.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
    SDL_RenderCopy(ren, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surf);
    SDL_RenderFillRect(ren, &point);
}

#endif
