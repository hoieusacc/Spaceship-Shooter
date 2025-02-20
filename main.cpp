#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include <random>

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

struct Object {
    float x, y, vx, vy, size, angle;
};

struct Player {
    float x, y, vx, vy, size, a;
    bool moving, fire;
};

struct Mouse{
    float x, y;
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
        std::cerr << "TTF không thể khởi tạo! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("JetBrainsMono-Regular.ttf", 40);
    if (font == nullptr) {
        std::cerr << "Không thể tải font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    

    Uint32 lastTime = SDL_GetTicks();
    float friction = 0.990f;
    int score = 0;
    bool start = false;
    bool run = true;
    int currentOption = 0;
    SDL_Event e;

    SDL_ShowCursor(SDL_DISABLE);
    while (run){
        Player player = {400, 300, 0, 0, 10, 200, 0, 0};
        
        Mouse mouse = {0, 0};
        int length = 6;
        float rad = PI / 2;
        float omega = 0.025;
        bool increase = true;
        
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
                        if (currentOption > 0){
                            currentOption--;
                        }
                        break;
                    case SDLK_DOWN:
                        if (currentOption < 3){
                            currentOption++;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (currentOption == 0){
                            std::cout << "Bat dau tro choi!" << std::endl;
                            score = 0;
                            start = true;
                            run = false;
                        }
                        else if (currentOption == 1) std::cout << "Cai dat tro choi!" << std::endl;
                        else if (currentOption == 2) std::cout << "Diem cao: " << score << std::endl;
                        else run = false;
                        break;
                    default:
                        break;
                }
            }
        }
        while (start) {
            auto frameStart = std::chrono::steady_clock::now();
            Uint32 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_KEYDOWN){
                    player.moving = true;
                    switch (e.key.keysym.sym){
                        case SDLK_a:
                            player.vx = 0;
                            player.vx -= player.a * deltaTime;
                            if (player.vx < MIN_VELOCITY){
                                player.vx = MIN_VELOCITY;
                            }
                            break;
                        case SDLK_d:
                            player.vx = 0;
                            player.vx += player.a * deltaTime;
                            if (player.vx > MAX_VELOCITY){
                                player.vx = MAX_VELOCITY;
                            }
                            break;
                        case SDLK_w:
                            player.vy = 0;
                            player.vy -= player.a * deltaTime;
                            if (player.vy < MIN_VELOCITY){
                                player.vy = MIN_VELOCITY;
                            }
                            break;
                        case SDLK_s:
                            player.vy = 0;
                            player.vy += player.a * deltaTime;
                            if (player.vy > MAX_VELOCITY){
                                player.vy = MAX_VELOCITY;
                            }
                            break;
                        case SDLK_f:
                            createEnemies(enemies, numberOfEnemies);
                            break;
                        case SDLK_ESCAPE:
                            start = false;
                            run = true;
                            break;
                    }
                }
                if (e.type == SDL_KEYUP){
                    player.moving = false;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        drawLineToMouse(player, renderer, e.button.x, e.button.y);
                        mouse.x = e.button.x;
                        mouse.y = e.button.y;
                        player.fire = true;

                    }
                    else if (e.button.button == SDL_BUTTON_RIGHT) {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        drawLineToMouse(player, renderer, e.button.x, e.button.y);
                        mouse.x = e.button.x;
                        mouse.y = e.button.y;
                        player.fire = true;
                    }
                }
            }

            updatePlayerPosition(player, WINDOW_WIDTH, WINDOW_HEIGHT, friction);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            float line = length + 8 * cos(rad) * cos(rad);
            rad += omega;
            if (rad >= 2 * PI){
                rad = 0;
            }
            SDL_RenderDrawLine(renderer, mouseX + line, mouseY, mouseX + line - 2, mouseY);
            SDL_RenderDrawLine(renderer, mouseX, mouseY + line, mouseX, mouseY + line - 2);
            SDL_RenderDrawLine(renderer, mouseX - line, mouseY, mouseX - line + 2, mouseY);
            SDL_RenderDrawLine(renderer, mouseX, mouseY - line, mouseX, mouseY - line + 2);

            drawCircle(renderer, player.x, player.y, player.size);

            for (int i = 2; i <= numberOfEnemies; i++) {
                Object* enemy = enemies.takeDataAtPosition(i);
            
                float velocity = 1;
            
                moveEnemyTowardsPlayer(*enemy, player, velocity);
            
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                drawCircle(renderer, enemy->x, enemy->y, enemy->size);
            
                if (colideCheck(*enemy, player)) {
                    std::cout << "Game over" << std::endl;
                    start = false;
                    run = true;
                }

                if (player.fire && isEnemyOnLine(player, mouse, *enemy)){
                    score += 10;
                    enemies.deleteAtPosition(i);
                    numberOfEnemies--;
                    if (numberOfEnemies == 1){
                        std::cout << "Wave Completed!" << std::endl;
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
        
        drawMenu(renderer, font, currentOption);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
