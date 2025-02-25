#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "commonVar.h"

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

#endif