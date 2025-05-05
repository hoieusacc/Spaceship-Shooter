#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "commonVar.h"

class BaseObject{
public:
    float x, y, vx, vy, size;
};

class Enemy : public BaseObject{
public:
    float angle;
    int v;
    int maxHealth;
    SDL_Rect dstRect;
    SDL_Rect srcRect;

    void destroyAnimation(SDL_Renderer* &renderer, SDL_Texture* &loadTexture){
        dstRect = {static_cast<int>(x - size / 2), static_cast<int>(y - size / 2), static_cast<int>(size * 1.5), static_cast<int>(size * 1.5)};
        for (int i = 0; i < 18 * 8; i++){
            srcRect = {i / 8 * 64, 0, 64, 64};
            SDL_RenderCopyEx(renderer, loadTexture, &srcRect, &dstRect, angle * 180 / PI, nullptr, SDL_FLIP_NONE);
        }
    }
};

class Player : public BaseObject{
public:
    float a, angle;
    bool moving, fire;
    int health;
    SDL_Rect raydstRect;
    SDL_Rect raysrcRect;
    SDL_Point point;

    void move(float friction){
        x += vx;
        y += vy;

        if (y > WINDOW_HEIGHT - size) {
            y = WINDOW_HEIGHT - size;
        }
        if (x < size) {
            x = size;
        }
        if (x > WINDOW_WIDTH - size) {
            x = WINDOW_WIDTH - size;
        }
        if (y < size) {
            y = size;
        }

        if (vx != 0 && !moving) {
            vx *= friction;
        }
        if (vy != 0 && !moving) {
            vy *= friction;
        }
    }

    void castRay(SDL_Renderer* &renderer, SDL_Texture* &loadTexture){
        for (int i = 0; i < 4 * 160; i++){
            raydstRect = {static_cast<int>(x - 18 / 2), static_cast<int>(y - 38), 18, 38 * 2};
            raysrcRect = {(i / 160) * 18, 0, 18, 38};
            //point = {static_cast<int>(x - 18 / 2), static_cast<int>(y - 38)};
            SDL_RenderCopyEx(renderer, loadTexture, &raysrcRect, &raydstRect, angle * 180 / PI, NULL, SDL_FLIP_NONE);
        }
    }
};

class Bullet{
public:
    float x, y;
    float angle;
    int size = 2;
    int v = 5;
    int currentFrame = 0;
    SDL_Rect dstRect = {x - (size * 9) / 2, y - (size * 16) / 2, size * 9, size * 16};
    SDL_Rect srcRect = {currentFrame * 9, 0, 9, 16};

    void move(float dx, float dy){
        x += v * dx;
        y += v * dy;
    }

    void update(){
        currentFrame = (currentFrame + 1) % 4;
        dstRect = {static_cast<int>(x - (size * 9) / 2),static_cast<int>(y - (size * 16) / 2), size * 9, size * 16};
        srcRect = {currentFrame * 9, 0, 9, 16};
    }
};

class Mouse : public BaseObject{
public:
    bool moving;
    float size;
    
    void move(float friction){
        x += vx;
        y += vy;

        if (y > WINDOW_HEIGHT - size) {
            y = WINDOW_HEIGHT - size;
        }
        if (x < size) {
            x = size;
        }
        if (x > WINDOW_WIDTH - size) {
            x = WINDOW_WIDTH - size;
        }
        if (y < size) {
            y = size;
        }

        if (vx != 0 && !moving) {
            vx *= friction;
        }
        if (vy != 0 && !moving) {
            vy *= friction;
        }
    }
};

struct Node{
    Enemy* data;
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

    void insertAtEnd(Enemy* value) {
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

    Enemy* takeDataAtPosition(int position){
        Node* temp = head;
        for (int i = 1; i < position && temp; ++i){
            temp = temp->next;
        }

        return temp->data;
    }
    
    void deleteAllEnemy(int numberOfEnemies){
        for (int i = numberOfEnemies; i > 1; i--){
            deleteAtPosition(i);
        }
        head->next = NULL;
    }
};

#endif