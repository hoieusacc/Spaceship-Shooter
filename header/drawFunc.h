#ifndef DRAW_FUNCTION_H_
#define DRAW_FUNTION_H_

#include "baseObject.h"
#include "commonVar.h"
#include "logicFunc.h"

void drawImage(SDL_Renderer* renderer, const char* path, SDL_Rect dstRect, SDL_Rect srcRect, double angle) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, angle, nullptr, SDL_FLIP_NONE);
}

void drawHealthBar(int start, int end, int y, int size, int health){
    int temp = health;
    SDL_Rect dstRect1 = {start - 2.5, y + 2.5,health * (end - start) / 4, size};
    SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
    SDL_RenderFillRect(renderer, &dstRect1);

    SDL_Rect dstRect2 = {start - 2.5, y, temp * (end - start) / 4 + 2.5, size + 5};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &dstRect2);
}

void drawMenu(SDL_Renderer* textRenderer, int currentOption) {
    TTF_Font* font = TTF_OpenFont("data/font/JetBrainsMono-Regular.ttf", 40);
    const char* menuItems[] = {"Start" ,"Setting" ,"High Score" ,"Quit"};

    SDL_Color white = { 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0 };

    for (int i = 0; i < 4; ++i) {
        surface = TTF_RenderUTF8_Solid(font, menuItems[i], (i == currentOption) ? yellow : white);
        texture = SDL_CreateTextureFromSurface(textRenderer, surface);
        int textWidth = surface->w;
        int textHeight = surface->h;
        SDL_FreeSurface(surface);

        SDL_Rect dstRect = { WINDOW_WIDTH / 2 - textWidth / 2, WINDOW_HEIGHT / 2 - 50 + (i - 1) * textHeight, textWidth, textHeight };
        SDL_RenderCopy(textRenderer, texture, nullptr, &dstRect);
    }
}

void drawSetting(SDL_Renderer* textRenderer, int currentOption, int volume) {
    TTF_Font* font = TTF_OpenFont("data/font/JetBrainsMono-Regular.ttf", 40);
    SDL_SetRenderDrawColor(textRenderer, 0, 0, 0, 255);
    SDL_RenderClear(textRenderer);

    const char* menuItems[] = {"Music" ,"Back"};

    SDL_Color white = { 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0 };

    for (int i = 0; i < 2; ++i) {
        surface = TTF_RenderUTF8_Solid(font, menuItems[i], (i == currentOption) ? yellow : white);
        texture = SDL_CreateTextureFromSurface(textRenderer, surface);
        int textWidth = surface->w;
        int textHeight = surface->h;
        SDL_FreeSurface(surface);
        SDL_Rect dstRect;
        if (i == 0 && currentOption == i){
            dstRect = {WINDOW_WIDTH / 2 - 2 * textWidth ,WINDOW_HEIGHT / 2 - 50 + (i - 1) * (textHeight), textWidth, textHeight };
            drawHealthBar(WINDOW_WIDTH / 2 - textWidth + 20, WINDOW_WIDTH / 2 - textWidth + 70, WINDOW_HEIGHT / 2 - 35 + (i - 1) * (textHeight), textHeight / 2, volume / 4);
        }
        else{
            dstRect = {(WINDOW_WIDTH - textWidth) / 2 ,WINDOW_HEIGHT / 2 - 50 + (i - 1) * (textHeight), textWidth, textHeight };
        }
        SDL_RenderCopy(textRenderer, texture, nullptr, &dstRect);
    }
}

void drawScore(SDL_Renderer* renderer, int score){
    TTF_Font* font = TTF_OpenFont("data/font/JetBrainsMono-Regular.ttf", 15);
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Color white = { 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0 };
    surface = TTF_RenderUTF8_Solid(font, scoreText.c_str(), white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {20 , 20, textWidth, textHeight };
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
}

void drawLineToMouse(Player &player, SDL_Renderer* renderer, int mouseX, int mouseY){
    SDL_RenderDrawLine(renderer, player.x , player.y , mouseX, mouseY);
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
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

void drawBackground(float posx){
    SDL_Rect srcRect = {posx, 0, 1080, 720};
    SDL_Rect dstRect = {0, 0, 1080, 720};
    drawImage(renderer, "data/image/BackGrounds/Condesed/Starry background  - Layer 01 - Void.png", dstRect, srcRect, 0);
    drawImage(renderer, "data/image/BackGrounds/Condesed/Starry background  - Layer 02 - Stars.png", dstRect, srcRect, 0);
    drawImage(renderer, "data/image/BackGrounds/Condesed/Starry background  - Layer 03 - Stars.png", dstRect, srcRect, 0);
}

#endif