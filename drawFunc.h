#ifndef DRAW_FUNCTION_H_
#define DRAW_FUNTION_H_

#include "baseObject.h"
#include "commonVar.h"
#include "logicFunc.h"

void drawMenu(SDL_Renderer* textRenderer, TTF_Font* font, int currentOption) {
    SDL_SetRenderDrawColor(textRenderer, 0, 0, 0, 255);
    SDL_RenderClear(textRenderer);

    const char* menuItems[] = {"Bắt đầu" ,"Cài đặt" ,"Điểm cao" ,"Thoát"};

    SDL_Color white = { 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0 };

    for (int i = 0; i < 4; ++i) {
        SDL_Surface* surface = TTF_RenderUTF8_Solid(font, menuItems[i], (i == currentOption) ? yellow : white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(textRenderer, surface);
        int textWidth = surface->w;
        int textHeight = surface->h;
        SDL_FreeSurface(surface);

        SDL_Rect dstRect = { WINDOW_WIDTH / 2 - textWidth / 2, WINDOW_HEIGHT / 2 + (i - 1) * textHeight, textWidth, textHeight };
        SDL_RenderCopy(textRenderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(textRenderer);
}

void drawSetting(SDL_Renderer* textRenderer, TTF_Font* font, int currentOption) {
    SDL_SetRenderDrawColor(textRenderer, 0, 0, 0, 255);
    SDL_RenderClear(textRenderer);

    const char* menuItems[] = {"Cài đặt" ,"Trở về"};

    SDL_Color white = { 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0 };

    for (int i = 0; i < 2; ++i) {
        SDL_Surface* surface = TTF_RenderUTF8_Solid(font, menuItems[i], (i == currentOption) ? yellow : white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(textRenderer, surface);
        int textWidth = surface->w;
        int textHeight = surface->h;
        SDL_FreeSurface(surface);

        SDL_Rect dstRect = {textWidth / 4 ,i * textHeight, textWidth, textHeight };
        SDL_RenderCopy(textRenderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(textRenderer);
}

void drawScore(SDL_Renderer* renderer, TTF_Font* font, int score){
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Color white = { 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0 };
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, scoreText.c_str(), white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {textWidth / 4 , textHeight, textWidth, textHeight };
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);
}

void drawLineToMouse(Player &player, SDL_Renderer* renderer, int mouseX, int mouseY){
    SDL_RenderDrawLine(renderer, player.x + player.size / 2, player.y + player.size / 2, mouseX, mouseY);
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

#endif