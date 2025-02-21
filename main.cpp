#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include <random>
#include <string>

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define MAX_VELOCITY 10
#define MIN_VELOCITY -10
#define FPS 120
#define FRAME_DELAY  1000 / FPS
#define PI 3.14159

SDL_Window* window = SDL_CreateWindow("Prototype Spaceship Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

void drawMenu(SDL_Renderer* renderer, TTF_Font* font, int currentOption) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const char* menuItems[] = {"Bắt đầu" ,"Cài đặt" ,"Điểm cao" ,"Thoát"};

    SDL_Color white = { 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0 };

    for (int i = 0; i < 4; ++i) {
        SDL_Surface* surface = TTF_RenderUTF8_Solid(font, menuItems[i], (i == currentOption) ? yellow : white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        int textWidth = surface->w;
        int textHeight = surface->h;
        SDL_FreeSurface(surface);

        SDL_Rect dstRect = { WINDOW_WIDTH / 2 - textWidth / 2, WINDOW_HEIGHT / 2 + (i - 1) * textHeight, textWidth, textHeight };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);
}

void drawSetting(SDL_Renderer* renderer, TTF_Font* font, int currentOption) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const char* menuItems[] = {"Cài đặt" ,"Trở về"};

    SDL_Color white = { 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0 };

    for (int i = 0; i < 2; ++i) {
        SDL_Surface* surface = TTF_RenderUTF8_Solid(font, menuItems[i], (i == currentOption) ? yellow : white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        int textWidth = surface->w;
        int textHeight = surface->h;
        SDL_FreeSurface(surface);

        SDL_Rect dstRect = {textWidth / 4 ,i * textHeight, textWidth, textHeight };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);
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

struct Object {
    float x, y, vx, vy, size, angle;
};

struct Player {
    float x, y, vx, vy, size, a;
    bool moving, fire;
};

struct Mouse{
    float x, y, vx, vy;
    bool moving;
    int size;
};

struct Node{
    Object* data;
    Node* next;
};

class LinkedList {
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current->data;
            delete current;
            current = nextNode;
        }
    }

    void insertAtEnd(Object* value) {
        Node* newNode = new Node{value};

        if (head == nullptr) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void deleteAtPosition(int position){
        Node* temp = head;
        for (int i = 1; i < position - 1 && temp; ++i){
            temp = temp->next;
        }

        Node* nodeToDelete = temp->next;
        temp->next = temp->next->next;
        delete nodeToDelete;
    }

    Object* takeDataAtPosition(int position){
        Node* temp = head;
        for (int i = 1; i < position && temp; ++i){
            temp = temp->next;
        }

        return temp->data;
    }
};

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

void updatePlayerPosition(Player& player, int windowWidth, int windowHeight, float friction) {
    player.x += player.vx;
    player.y += player.vy;

    if (player.y > windowHeight + player.size) {
        player.y = -player.size;
    }
    if (player.x < -player.size) {
        player.x = windowWidth + player.size;
    }
    if (player.x > windowWidth + player.size) {
        player.x = -player.size;
    }
    if (player.y < -player.size) {
        player.y = windowHeight + player.size;
    }

    if (player.vx != 0 && !player.moving) {
        player.vx *= friction;
    }
    if (player.vy != 0 && !player.moving) {
        player.vy *= friction;
    }
}

void updateMousePosition(Mouse& mouse, int windowWidth, int windowHeight, float friction) {
    mouse.x += mouse.vx;
    mouse.y += mouse.vy;

    if (mouse.y > windowHeight - mouse.size) {
        mouse.y = windowHeight - mouse.size;
    }
    if (mouse.x < mouse.size) {
        mouse.x = mouse.size;
    }
    if (mouse.x > windowWidth - mouse.size) {
        mouse.x = windowWidth - mouse.size;
    }
    if (mouse.y < mouse.size) {
        mouse.y = mouse.size;
    }

    if (mouse.vx != 0 && !mouse.moving) {
        mouse.vx *= friction;
    }
    if (mouse.vy != 0 && !mouse.moving) {
        mouse.vy *= friction;
    }
}

void moveEnemyTowardsPlayer(Object& enemy, const Player& player, float speed) {
    float directionX = player.x - enemy.x;
    float directionY = player.y - enemy.y;

    float length = std::sqrt(directionX * directionX + directionY * directionY);

    directionX /= length;
    directionY /= length;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, enemy.x, enemy.y, enemy.x + directionX * 40, enemy.y + directionY * 40);

    enemy.x += directionX * speed;
    enemy.y += directionY * speed;
}

bool colideCheck(Object enemy, Player player){
    float directionX = player.x - enemy.x;
    float directionY = player.y - enemy.y;

    float length = std::sqrt(directionX * directionX + directionY * directionY);

    return (length - enemy.size - player.size) < 0;
}

int getRamdomNumber(int start, int end){
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_int_distribution<> distr(start, end);

    return distr(gen);
}

void createEnemies(LinkedList &enemies, int &numberOfEnemies){
    Object* temp = new Object{getRamdomNumber(0, WINDOW_WIDTH), getRamdomNumber(0, WINDOW_HEIGHT), 0, 0, 10, 0};
    enemies.insertAtEnd(temp);
    numberOfEnemies++;
}

bool isEnemyOnLine(const Player& player, const Mouse& mouse, const Object& enemy) {
    float dx = mouse.x - player.x;
    float dy = mouse.y - player.y;
    
    float distance = std::abs(dy * enemy.x - dx * enemy.y + mouse.x * player.y - mouse.y * player.x) / std::sqrt(dy * dy + dx * dx);

    float dX = enemy.x - player.x;
    float dY = enemy.y - player.y;
    
    if (distance <= enemy.size){
        if (dx < 0 && dy < 0){
            if (dX < 0 && dY < 0){
                if ((dx < dX) && (dy < dY)){
                    return true;
                }
            }
        }
        if (dx > 0 && dy < 0){
            if (dX > 0 && dY < 0){
                if ((dx > dX) && (dy < dY)){
                    return true;
                }
            }
        }
        if (dx > 0 && dy > 0){
            if (dX > 0 && dY > 0){
                if ((dx > dX) && (dy > dY)){
                    return true;
                }
            }
        }
        if (dx < 0 && dy > 0){
            if (dX < 0 && dY > 0){
                if ((dx < dX) && (dy > dY)){
                    return true;
                }
            }
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    Uint32 lastTime = SDL_GetTicks();
    float friction = 0.95f;
    int highScore = 0;
    bool startGame = false;
    bool startSetting = false;
    bool run = true;
    int menuOption = 0;
    int settingOption = 0;
    SDL_Event e;

    SDL_ShowCursor(SDL_DISABLE);
    while (run){
        Player player = {400, 300, 0, 0, 10, 200, 0, 0};
        
        int tempx, tempy;
        int length = 6;
        SDL_GetMouseState(&tempx, &tempy);
        Mouse mouse = {tempx, tempy, 0, 0, 0, length + 8};
        float crossFireFriction = 0.90f;
        float rad = PI / 2;
        float omega = 0.025;
        bool increase = true;
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
                            std::cout << "Bat dau tro choi!" << std::endl;
                            startGame = true;
                            run = false;
                        }
                        else if (menuOption == 1){ 
                            std::cout << "Cai dat tro choi!" << std::endl;
                            startSetting = true;
                            run = false;
                        }
                        else if (menuOption == 2) std::cout << "Diem cao: " << highScore << std::endl;
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
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            updatePlayerPosition(player, WINDOW_WIDTH, WINDOW_HEIGHT, friction);
            drawCircle(renderer, player.x, player.y, player.size);
            updateMousePosition(mouse, WINDOW_WIDTH, WINDOW_HEIGHT, crossFireFriction);
            drawCrosshair(mouse, renderer, length, rad, omega);
            TTF_Font* font = TTF_OpenFont("JetBrainsMono-Regular.ttf", 15);
            drawScore(renderer, font, score);

            for (int i = 2; i <= numberOfEnemies; i++) {
                Object* enemy = enemies.takeDataAtPosition(i);
            
                float velocity = 1;
            
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
            TTF_Font* font = TTF_OpenFont("JetBrainsMono-Regular.ttf", 20);
            drawSetting(renderer, font, settingOption);
        }
        TTF_Font* font = TTF_OpenFont("JetBrainsMono-Regular.ttf", 40);
        drawMenu(renderer, font, menuOption);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
