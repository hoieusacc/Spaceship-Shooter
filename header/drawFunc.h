#ifndef DRAW_FUNCTION_H_
#define DRAW_FUNTION_H_

#include "baseObject.h"
#include "commonVar.h"
#include "logicFunc.h"

int textWidth;
int textHeight;

SDL_Color white = { 255, 255, 255 };
SDL_Color yellow = { 255, 255, 0 };

TTF_Font* menuFont = NULL;
TTF_Font* settingFont = NULL;
TTF_Font* scoreFont = NULL;

SDL_Rect menuRect;
SDL_Rect settingRect;
SDL_Rect settingRect2;
SDL_Rect scoreRect;
SDL_Rect srcBackgroundRect[3];
SDL_Rect dstBackgroundRect;

const char* menuItems[] = {"Start" ,"Setting" ,"High Score" ,"Quit"};
const char* settingItems[] = {"Music", "Sensitivity", "Game Mode","Back"};
const char* mode[] = {"Easy", "Normal", "Hard"};

void loadImage(SDL_Surface* &loadSurface, SDL_Texture* &loadTexture, const char* &path){
    loadSurface = IMG_Load(path);
    loadTexture = SDL_CreateTextureFromSurface(renderer, loadSurface);

    SDL_FreeSurface(loadSurface);
}

void loadBackground(){
    for (int i = 0; i < 3; i++){
        loadImage(backgroundSurface[i], backgroundTexture[i], backgroundPath[i]);
    }
}

void drawImage(SDL_Renderer* &renderer, const char* path, SDL_Rect dstRect, SDL_Rect srcRect, double angle) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    
    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, angle, nullptr, SDL_FLIP_NONE);
}

void drawImage2(SDL_Renderer* &renderer, SDL_Texture* &loadTexture, SDL_Rect &dstRect, SDL_Rect &srcRect, float angle) {
    SDL_RenderCopyEx(renderer, loadTexture, &srcRect, &dstRect, angle, nullptr, SDL_FLIP_NONE);
}


template <class T>
void drawHealthBar(T start, T end, T y, T size, T health){
    TTF_Font* font = TTF_OpenFont("data/font/Vermin Vibes 1989.ttf", 50);

    SDL_Rect dstRect1 = {start - 2.5, y + 2.5,health * (end - start) / 4, size};
    SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
    SDL_RenderFillRect(renderer, &dstRect1);

    SDL_Color white = { 255, 255, 255 };
    std::string healthText = std::to_string(health);
    SDL_Surface* surfaceText = TTF_RenderUTF8_Solid(font, healthText.c_str(), white);
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(backgroundRenderer, surfaceText);
    int textWidth = surfaceText->w;
    int textHeight = surfaceText->h;

    SDL_Rect dstRect3 = {start + 10 , y, textWidth, textHeight };
    SDL_RenderCopy(renderer, textureText, nullptr, &dstRect3);

    SDL_Rect dstRect2 = {start - 2.5, y, health * (end - start) / 4 + 2.5, size + 5};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &dstRect2);
}

void drawMenu(SDL_Renderer* &backgroundRenderer, int currentOption) {
    menuFont = TTF_OpenFont("data/font/Vermin Vibes 1989.ttf", 50);

    surface = TTF_RenderUTF8_Solid(menuFont, "Spaceship Shooter", white);
    texture = SDL_CreateTextureFromSurface(backgroundRenderer, surface);
    textWidth = surface->w;
    textHeight = surface->h;
    SDL_FreeSurface(surface);
    menuRect = { WINDOW_WIDTH / 2 - textWidth / 2, 150, textWidth, textHeight };
    SDL_RenderCopy(backgroundRenderer, texture, nullptr, &menuRect);

    menuFont = TTF_OpenFont("data/font/Vermin Vibes 1989.ttf", 40);

    for (int i = 0; i < 4; ++i) {
        surface = TTF_RenderUTF8_Solid(menuFont, menuItems[i], (i == currentOption) ? yellow : white);
        texture = SDL_CreateTextureFromSurface(backgroundRenderer, surface);
        textWidth = surface->w;
        textHeight = surface->h;
        SDL_FreeSurface(surface);

        menuRect = { WINDOW_WIDTH / 2 - textWidth / 2, WINDOW_HEIGHT / 2 + (i - 1) * textHeight, textWidth, textHeight };
        SDL_RenderCopy(backgroundRenderer, texture, nullptr, &menuRect);
    }
}

void drawSetting(SDL_Renderer* &backgroundRenderer, int &currentOption, int &volume, int sensitivity, int gameMode) {
    settingFont = TTF_OpenFont("data/font/Vermin Vibes 1989.ttf", 40);
    SDL_SetRenderDrawColor(backgroundRenderer, 0, 0, 0, 255);
    SDL_RenderClear(backgroundRenderer);

    for (int i = 0; i < 4; ++i) {
        surface = TTF_RenderUTF8_Solid(settingFont, settingItems[i], (i == currentOption) ? yellow : white);
        texture = SDL_CreateTextureFromSurface(backgroundRenderer, surface);
        textWidth = surface->w;
        textHeight = surface->h;
        SDL_FreeSurface(surface);
        if (i == 0 && currentOption == i){
            settingRect = {WINDOW_WIDTH / 2 - textWidth - 20 ,WINDOW_HEIGHT / 2 - 50 + (i - 1) * (textHeight), textWidth, textHeight };
            drawHealthBar(WINDOW_WIDTH / 2 + 10, WINDOW_WIDTH / 2 + 70, WINDOW_HEIGHT / 2 - 45 + (i - 1) * (textHeight), textHeight / 2, volume / 4);
        }
        else if (i == 1 && currentOption == i){
            settingRect = {WINDOW_WIDTH / 2 - textWidth - 20 ,WINDOW_HEIGHT / 2 - 50 + (i - 1) * (textHeight), textWidth, textHeight };
            drawHealthBar(WINDOW_WIDTH / 2 + 10, WINDOW_WIDTH / 2 + 70, WINDOW_HEIGHT / 2 - 45 + (i - 1) * (textHeight), textHeight / 2, sensitivity / 8);
        }
        else if (i == 2 && currentOption == i){
            settingRect = {WINDOW_WIDTH / 2 - textWidth - 20 ,WINDOW_HEIGHT / 2 - 50 + (i - 1) * (textHeight), textWidth, textHeight };
            modeSurface = TTF_RenderUTF8_Solid(settingFont, mode[gameMode - 1], yellow);
            modeTexture = SDL_CreateTextureFromSurface(backgroundRenderer, modeSurface);
            SDL_FreeSurface(modeSurface);
            settingRect2 = {WINDOW_WIDTH / 2 ,WINDOW_HEIGHT / 2 - 50 + (i - 1) * (textHeight), textWidth, textHeight };
        }
        else{
            settingRect = {(WINDOW_WIDTH - textWidth) / 2 ,WINDOW_HEIGHT / 2 - 50 + (i - 1) * (textHeight), textWidth, textHeight };
        }
        if (currentOption != 2){
            modeTexture = NULL;
        }
        SDL_RenderCopy(backgroundRenderer, texture, nullptr, &settingRect);
        SDL_RenderCopy(backgroundRenderer, modeTexture, nullptr, &settingRect2);
    }
}

void drawScore(SDL_Renderer* &renderer, int &score){
    scoreFont = TTF_OpenFont("data/font/Vermin Vibes 1989.ttf", 15);
    std::string scoreText = "Score: " + std::to_string(score);
    surface = TTF_RenderUTF8_Solid(scoreFont, scoreText.c_str(), white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    textWidth = surface->w;
    textHeight = surface->h;
    SDL_FreeSurface(surface);

    scoreRect = {20 , 20, textWidth, textHeight };
    SDL_RenderCopy(renderer, texture, nullptr, &scoreRect);
}

void drawLineToMouse(Player &player, SDL_Renderer* &renderer, Mouse& mouse){
    SDL_RenderDrawLine(renderer, player.x , player.y , mouse.x, mouse.y);
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}

void drawCircle(SDL_Renderer* &renderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if ((x * x) + (y * y) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

void drawCrosshair(Mouse &mouse, SDL_Renderer* renderer, int length, float &rad, float omega){
    float line = length + 8 * cos(rad) * cos(rad);
    rad += omega;
    if (rad >= 2 * PI){
        rad = 0;
    }
    SDL_RenderDrawLine(renderer, mouse.x + line, mouse.y, mouse.x + line - 2, mouse.y);
    SDL_RenderDrawLine(renderer, mouse.x, mouse.y + line, mouse.x, mouse.y + line - 2);
    SDL_RenderDrawLine(renderer, mouse.x - line, mouse.y, mouse.x - line + 2, mouse.y);
    SDL_RenderDrawLine(renderer, mouse.x, mouse.y - line, mouse.x, mouse.y - line + 2);
}

void drawBackground(float layer1, float layer2, float layer3){
    srcBackgroundRect[0] = {(int)layer1, 0, 1080, 720};
    srcBackgroundRect[1] = {(int)layer2, 0, 1080, 720};
    srcBackgroundRect[2] = {(int)layer3, 0, 1080, 720};
    dstBackgroundRect = {0, 0, 1080, 720};

    for (int i = 0; i < 3; i++){
        drawImage2(renderer, backgroundTexture[i], dstBackgroundRect, srcBackgroundRect[i], 0);
    }
}

#endif