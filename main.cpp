#include "header/includeFile.h"

bool init(){
    window = SDL_CreateWindow("Prototype Spaceship Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window){
        std::cerr << "Cannot create window!" << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer){
        std::cerr << "Cannot create renderer!" << std::endl;
        return false;
    }
    textRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }
    
    return true;
}

void close(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!init()){
        std::cerr << "Cannot initialize SDL!" << std::endl;
    }
    Mix_Chunk* mainSong = Mix_LoadWAV("data/music/track 01/v1.0 full song.wav");
    if (mainSong == nullptr) {
        return -1;
    }

    Mix_PlayChannel(-1, mainSong, 0);
/*
    SDL_Surface* loadedSurface = IMG_Load("data/image/Main Ship/Main Ship - Base - Full health.png");
    if (loadedSurface == nullptr) {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return -1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (texture == nullptr) {
        printf("Unable to create texture! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return -1;
    }*/

    Uint32 lastTime = SDL_GetTicks();
    SDL_ShowCursor(SDL_DISABLE);

    while (run){
        Player player = {400, 300, 0, 0, 24, 200, 0, 0, 0};
        
        Mouse mouse = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0, 0, 0, length + 8};
        int score = 0;
        LinkedList enemies;
        int numberOfEnemies = 1;
        Object* temp = new Object{WINDOW_WIDTH - 1000, WINDOW_HEIGHT - 1000, 0, 0, 10, 0};
        enemies.insertAtEnd(temp);

        for (int i = 0; i < 4; i++){
            createEnemies(enemies, numberOfEnemies);
        }
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (menuOption > 0){
                            menuOption--;
                        }
                        break;
                    case SDLK_DOWN:
                        if (menuOption < 3){
                            menuOption++;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (menuOption == 0){
                            std::cout << "Game Start!" << std::endl;
                            startGame = true;
                            run = false;
                        }
                        else if (menuOption == 1){ 
                            std::cout << "Game Setting!" << std::endl;
                            startSetting = true;
                            run = false;
                        }
                        else if (menuOption == 2) std::cout << "High Score: " << highScore << std::endl;
                        else run = false;
                        break;
                    default:
                        break;
                }
            }
        }
        while (startGame) {
            auto frameStart = std::chrono::steady_clock::now();
            Uint32 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_KEYDOWN){
                    switch (e.key.keysym.sym){
                        case SDLK_a:
                            player.vx = 0;
                            player.vx -= player.a * deltaTime;
                            if (player.vx < MIN_VELOCITY){
                                player.vx = MIN_VELOCITY;
                            }
                            player.moving = true;
                            break;
                        case SDLK_d:
                            player.vx = 0;
                            player.vx += player.a * deltaTime;
                            if (player.vx > MAX_VELOCITY){
                                player.vx = MAX_VELOCITY;
                            }
                            player.moving = true;
                            break;
                        case SDLK_w:
                            player.vy = 0;
                            player.vy -= player.a * deltaTime;
                            if (player.vy < MIN_VELOCITY){
                                player.vy = MIN_VELOCITY;
                            }
                            player.moving = true;
                            break;
                        case SDLK_s:
                            player.vy = 0;
                            player.vy += player.a * deltaTime;
                            if (player.vy > MAX_VELOCITY){
                                player.vy = MAX_VELOCITY;
                            }
                            player.moving = true;
                            break;
                        case SDLK_f:
                            createEnemies(enemies, numberOfEnemies);
                            break;
                        case SDLK_UP:
                            mouse.vy = 0;
                            mouse.vy -= 1.5 * player.a * deltaTime;
                            if (mouse.vy < MIN_VELOCITY){
                                mouse.vy = MIN_VELOCITY;
                            }
                            mouse.moving = true;
                            break;
                        case SDLK_DOWN:
                            mouse.vy = 0;
                            mouse.vy += 1.5 * player.a * deltaTime;
                            if (mouse.vy > MAX_VELOCITY){
                                mouse.vy = MAX_VELOCITY;
                            }
                            mouse.moving = true;
                            break;
                        case SDLK_RIGHT:
                            mouse.vx = 0;
                            mouse.vx += 1.5 * player.a * deltaTime;
                            if (mouse.vx > MAX_VELOCITY){
                                mouse.vx = MAX_VELOCITY;
                            }
                            mouse.moving = true;
                            break;
                        case SDLK_LEFT:
                            mouse.vx = 0;
                            mouse.vx -= 1.5 * player.a * deltaTime;
                            if (mouse.vx < MIN_VELOCITY){
                                mouse.vx = MIN_VELOCITY;
                            }
                            mouse.moving = true;
                            break;
                        case SDLK_q:
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            drawLineToMouse(player, renderer, mouse.x, mouse.y);
                            player.fire = true;
                            break;
                        case SDLK_ESCAPE:
                            startGame = false;
                            run = true;
                            break;
                    }
                }
                if (e.type == SDL_KEYUP){
                    auto key = e.key.keysym.sym;
                    if (key == SDLK_a || key == SDLK_w || key == SDLK_d || key == SDLK_s){
                        player.moving = false;
                    }
                    if (key == SDLK_UP || key == SDLK_DOWN || key == SDLK_RIGHT || key == SDLK_LEFT){
                        mouse.moving = false;
                    }
                }
            }

            getAngle(player, mouse);
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            TTF_Font* font = TTF_OpenFont("data/font/JetBrainsMono-Regular.ttf", 15);
            drawScore(textRenderer, font, score);
            updatePlayerPosition(player, WINDOW_WIDTH, WINDOW_HEIGHT, friction);

            SDL_Rect srcRect = {0, 0, 48, 48};
            SDL_Rect dstRect = {player.x - 20,  player.y - 24, 48, 48};
            drawImage(renderer, "data/image/Main Ship/Main Ship - Base - Full health.png", dstRect, srcRect, player.angle * 180 / PI);

            updateMousePosition(mouse, WINDOW_WIDTH, WINDOW_HEIGHT, crossFireFriction);
            drawCrosshair(mouse, renderer, length, rad, omega);

            for (int i = 2; i <= numberOfEnemies; i++) {
                Object* enemy = enemies.takeDataAtPosition(i);
            
                moveEnemyTowardsPlayer(*enemy, player, velocity);
            
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                drawCircle(renderer, enemy->x, enemy->y, enemy->size);
            
                if (colideCheck(*enemy, player)) {
                    std::cout << "Game over" << std::endl;
                    startGame = false;
                    run = true;
                }

                if (player.fire && isEnemyOnLine(player, mouse, *enemy)){
                    score += 10;
                    enemies.deleteAtPosition(i);
                    numberOfEnemies--;
                    if (numberOfEnemies == 1){
                        std::cout << "Wave Completed!" << std::endl;
                    }
                    if (score > highScore){
                        highScore = score;
                    }
                }
            }
        
            player.fire = false;
            SDL_RenderPresent(renderer);

            auto frameEnd = std::chrono::steady_clock::now();
            std::chrono::duration<float, std::milli> elapsed = frameEnd - frameStart;
            if (elapsed.count() < FRAME_DELAY) {
                std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_DELAY) - elapsed);
            }
        }
        while (startSetting){
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            if (settingOption > 0){
                                settingOption--;
                            }
                            break;
                        case SDLK_DOWN:
                            if (settingOption < 1){
                                settingOption++;
                            }
                            break;
                        case SDLK_RIGHT:
                            if (settingOption == 0){
                                std::cout << "Cai dat tro choi!" << std::endl;
                            }
                            else{
                                run = true;
                                startSetting = false;
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            TTF_Font* font = TTF_OpenFont("data/font/JetBrainsMono-Regular.ttf", 20);
            drawSetting(renderer, font, settingOption);
        }
        SDL_SetRenderDrawColor(textRenderer, 0, 0, 0, 255);
        SDL_RenderClear(textRenderer);
        drawBackground();
        TTF_Font* font = TTF_OpenFont("data/font/JetBrainsMono-Regular.ttf", 40);
        drawMenu(renderer, font, menuOption);
    }
    close();

    return 0;
}