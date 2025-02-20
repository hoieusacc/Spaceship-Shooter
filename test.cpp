#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720

void drawMenu(SDL_Renderer* renderer, TTF_Font* font, int currentOption) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Menu items
    const char* menuItems[] = {"Bắt đầu" , "Cài đặt" ,"Thoát"};

    // Colors
    SDL_Color white = { 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0 };

    for (int i = 0; i < 3; ++i) {
        SDL_Surface* surface = TTF_RenderUTF8_Solid(font, menuItems[i], (i == currentOption) ? yellow : white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        int textWidth = surface->w;
        int textHeight = surface->h;
        SDL_FreeSurface(surface);

        SDL_Rect dstRect = { WINDOW_WIDTH / 2 - textWidth / 2, WINDOW_HEIGHT / 2 + (i - 1) * textHeight, textWidth, textHeight };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
    }

    // Update screen
    SDL_RenderPresent(renderer);
}

SDL_Window* window = SDL_CreateWindow("Prototype Spaceship Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

int main(int argc, char* argv[]) {
    // Initialize SDL and TTF
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL không thể khởi tạo! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() == -1) {
        std::cerr << "TTF không thể khởi tạo! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("JetBrainsMono-Regular.ttf", 40);
    if (font == nullptr) {
        std::cerr << "Không thể tải font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop
    bool quit = false;
    SDL_Event e;
    int currentOption = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (currentOption > 0){
                            currentOption--;
                        }
                        break;
                    case SDLK_DOWN:
                        if (currentOption < 2){
                            currentOption++;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (currentOption == 0) std::cout << "Bat dau tro choi!" << std::endl;
                        else if (currentOption == 1) std::cout << "Cai dat tro choi!" << std::endl;
                        else quit = true;
                        break;
                    default:
                        break;
                }
            }
        }

        drawMenu(renderer, font, currentOption);
    }

    // Clean up
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
