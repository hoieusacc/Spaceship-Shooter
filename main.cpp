#include "header/includeFile.h"

bool init(){
    window = SDL_CreateWindow("Spaceship Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
    SDL_DestroyRenderer(textRenderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
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

    Mix_PlayChannel(-1, mainSong, 1);
    Uint32 lastTime = SDL_GetTicks();
    SDL_ShowCursor(SDL_DISABLE);

    while (run){
        Player player = {400, 300, 0, 0, 32, 200, 0, 0, 0};
        Mouse mouse = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0, 0, 0, length + 8};
        int highScore = 0;
        int score = 0;

        LinkedList enemies;
        Enemy* temp = new Enemy{WINDOW_WIDTH - 1000, WINDOW_HEIGHT - 1000, 0, 0, 32, 0};
        enemies.insertAtEnd(temp);
        int numberOfEnemies = 1;

        int create = 4;
        for (int i = 0; i < create; i++){
            createEnemies(enemies, numberOfEnemies, temp->size);
        }

        Mix_Volume(-1, volume * 128 / 100);

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
                    case SDLK_RETURN:
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
        int move = 1;
        std::vector<Bullet> bullets;
        while (startGame) {
            auto frameStart = std::chrono::steady_clock::now();
            Uint32 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;
            posx = (posx * player.x) / 5760;

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
                        case SDLK_UP:
                            mouse.vy = 0;
                            mouse.vy -= sensitivity / 100 * player.a * deltaTime;
                            if (mouse.vy < MIN_VELOCITY){
                                mouse.vy = MIN_VELOCITY;
                            }
                            mouse.moving = true;
                            break;
                        case SDLK_DOWN:
                            mouse.vy = 0;
                            mouse.vy += sensitivity / 100 * player.a * deltaTime;
                            if (mouse.vy > MAX_VELOCITY){
                                mouse.vy = MAX_VELOCITY;
                            }
                            mouse.moving = true;
                            break;
                        case SDLK_RIGHT:
                            mouse.vx = 0;
                            mouse.vx += sensitivity / 100 * player.a * deltaTime;
                            if (mouse.vx > MAX_VELOCITY){
                                mouse.vx = MAX_VELOCITY;
                            }
                            mouse.moving = true;
                            break;
                        case SDLK_LEFT:
                            mouse.vx = 0;
                            mouse.vx -= sensitivity / 100 * player.a * deltaTime;
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
                        case SDLK_e:{
                            Bullet newBullet = {player.x, player.y, player.angle - PI / 2};
                            bullets.push_back(newBullet);
                            break;
                        }
                        case SDLK_f:
                            move += 1;
                            move %= 2;
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
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            
            getPlayerAngle(player, mouse);
            updatePlayerPosition(player, WINDOW_WIDTH, WINDOW_HEIGHT, friction);
            updateMousePosition(mouse, WINDOW_WIDTH, WINDOW_HEIGHT, crossFireFriction);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            for (int i = 0; i < bullets.size(); i++){
                drawCircle(renderer, bullets[i].x, bullets[i].y, bullets[i].size);
                bullets[i].move(cos(bullets[i].angle), sin(bullets[i].angle));
                if (bullets[i].x > WINDOW_WIDTH || bullets[i].x < 0 || bullets[i].y > WINDOW_HEIGHT || bullets[i].y < 0){
                    bullets.erase(bullets.begin() + i);
                }
            }
            
            SDL_Rect srcRect = {0, 0, 32, 32};
            SDL_Rect dstRect = {player.x - player.size / 2,  player.y - player.size / 2, player.size, player.size};
            
            drawScore(renderer, score);
            drawImage(renderer, state[player.health - 1], dstRect, srcRect, player.angle * 180 / PI);
            drawCrosshair(mouse, renderer, length, rad, omega);

            for (int i = 2; i <= numberOfEnemies; i++) {
                Enemy* enemy = enemies.takeDataAtPosition(i);
                if (move){
                    moveEnemyTowardsPlayer(*enemy, player, velocity);
                }
                getEnemyAngle(*enemy, player);
            
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect dstRect = {enemy->x - enemy->size / 2, enemy->y - enemy->size / 2, enemy->size, enemy->size};
                drawImage(renderer, "data/image/Enemy/Basic/Nairan - Fighter - Base.png", dstRect, srcRect, enemy->angle * 180 / PI);
            
                if (colideCheck(*enemy, player)) {
                    enemies.deleteAtPosition(i);
                    numberOfEnemies--;
                    player.health--;
                }

                if (player.fire && isEnemyOnLine(player, mouse, *enemy)){
                    score += 10;
                    enemies.deleteAtPosition(i);
                    numberOfEnemies--;
                    
                }
            }

            drawHealthBar(WINDOW_WIDTH - 320, WINDOW_WIDTH - 20, 20, 20, player.health);

            if (numberOfEnemies == 1){
                create++;
                for (int i = 0; i < create; i++){
                    createEnemies(enemies, numberOfEnemies, temp->size);
                }
            }
            if (!player.health){
                startGame = false;
                run = true;
                if (score > highScore){
                    highScore = score;
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
                            if (settingOption < 2){
                                settingOption++;
                            }
                            break;
                        case SDLK_RETURN:
                            if (settingOption == 2){
                                run = true;
                                startSetting = false;
                            }
                            break;
                        case SDLK_RIGHT:
                            if (settingOption == 0){
                                if (volume < 100){
                                    volume += 5;
                                }
                                Mix_Volume(-1, volume * 128 / 100);
                            }
                            else if (settingOption == 1){
                                if (sensitivity < 200){
                                    sensitivity +=  10;
                                }
                            }
                            break;
                        case SDLK_LEFT:
                            if (settingOption == 0){
                                if (volume > 0){
                                    volume -= 5;
                                }
                                Mix_Volume(-1, volume * 128 / 100);
                            }
                            else if (settingOption == 1){
                                if (sensitivity > 0){
                                    sensitivity -= 10;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            drawSetting(renderer, settingOption, volume, sensitivity);
            SDL_RenderPresent(renderer);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //drawBackground(posx);
        drawMenu(renderer, menuOption);
        SDL_RenderPresent(renderer);
    }
    close();
    return 0;
}