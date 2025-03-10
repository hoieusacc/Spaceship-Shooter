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
};

class Player : public BaseObject{
public:
    float a, angle;
    bool moving, fire;
    int health = 4;
};

class Bullet{
public:
    float x, y, angle;
    float size = 5;

    void move(float dx, float dy){
        x += 2 * dx;
        y += 2 * dy;
    }

};

class Mouse : public BaseObject{
public:
    bool moving;
    float size;
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
};

#endif