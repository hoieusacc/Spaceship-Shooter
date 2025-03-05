#ifndef LOGIC_FUNCTION_H_
#define LOGIC_FUNCTION_H_
#include "commonVar.h"
#include "baseObject.h"

void updatePlayerPosition(Player& player, int windowWidth, int windowHeight, float friction) {
    player.x += player.vx;
    player.y += player.vy;

    if (player.y > windowHeight - player.size) {
        player.y = windowHeight - player.size;
    }
    if (player.x < player.size) {
        player.x = player.size;
    }
    if (player.x > windowWidth - player.size) {
        player.x = windowWidth - player.size;
    }
    if (player.y < player.size) {
        player.y = player.size;
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

void moveEnemyTowardsPlayer(Enemy& enemy, const Player& player, float speed) {
    float directionX = player.x - enemy.x;
    float directionY = player.y - enemy.y;

    float length = std::sqrt(directionX * directionX + directionY * directionY);

    directionX /= length;
    directionY /= length;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, enemy.x, enemy.y, enemy.x + directionX * 40 , enemy.y + directionY * 40);

    enemy.x += directionX * speed;
    enemy.y += directionY * speed;
}

bool colideCheck(Enemy enemy, Player player){
    float directionX = player.x - enemy.x;
    float directionY = player.y - enemy.y;

    float length = std::sqrt(directionX * directionX + directionY * directionY);

    return (length - enemy.size - player.size + enemy.size / 4 + player.size / 4) < 0;
}

float getRandomNumber(int start, int end){
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_int_distribution<> distr(start, end);

    return (float)distr(gen);
}

void createEnemies(LinkedList &enemies, int &numberOfEnemies, float size){
    int choice = getRandomNumber(1, 4);
    Enemy* temp;
    switch (choice){
        case 1:
            temp = new Enemy{getRandomNumber(0, WINDOW_WIDTH), getRandomNumber(WINDOW_HEIGHT, WINDOW_HEIGHT + 200), 0, 0, size, 0};
            break;
        case 2:
            temp = new Enemy{getRandomNumber(0, WINDOW_WIDTH), getRandomNumber(-200, 0), 0, 0, size, 0};
            break;
        case 3:
            temp = new Enemy{getRandomNumber(WINDOW_WIDTH, WINDOW_WIDTH + 200), getRandomNumber(0, WINDOW_HEIGHT), 0, 0, size, 0};
            break;
        case 4:
            temp = new Enemy{getRandomNumber(-200, 0), getRandomNumber(0, WINDOW_HEIGHT), 0, 0, size, 0};
            break;
        
    }
    enemies.insertAtEnd(temp);
    numberOfEnemies++;
}

bool isEnemyOnLine(const Player& player, const Mouse& mouse, const Enemy& enemy) {
    float dx = mouse.x - player.x;
    float dy = mouse.y - player.y;
    
    float distance = std::abs(dy * enemy.x - dx * enemy.y + mouse.x * player.y - mouse.y * player.x) / std::sqrt(dy * dy + dx * dx) + enemy.size / 3;

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

float getDistance(const Player player, const Mouse mouse){
    return sqrt((mouse.x - player.x) * (mouse.x - player.x) + (mouse.y - player.y) * (mouse.y - player.y));
}

void getPlayerAngle(Player& player, Mouse& mouse){
    double deltaX = mouse.x - player.x;
    double deltaY = mouse.y - player.y;

    double angle = atan2(deltaY, deltaX) + PI / 2;

    if (angle < 0) {
        angle += 2 * PI;
    }
    player.angle = angle;
}

void getEnemyAngle(Enemy& enemy, Player player){
    double deltaX = player.x - enemy.x;
    double deltaY = player.y - enemy.y;

    double angle = atan2(deltaY, deltaX) + PI / 2;

    if (angle < 0) {
        angle += 2 * PI;
    }
    enemy.angle = angle;
}

#endif