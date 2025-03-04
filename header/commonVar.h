#ifndef COMMON_VARIABLE_H_
#define COMMON_VARIABLE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include <random>
#include <string>
#include <windows.h>

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const int BPP = 32;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Renderer* textRenderer = NULL;
static SDL_Texture* texture = NULL;
static SDL_Surface* surface = NULL;
SDL_Event e;

const float PI = 3.14159; 
const int FPS = 120;
const int FRAME_DELAY = 1000 / FPS;
const int MAX_VELOCITY = 7;
const int MIN_VELOCITY = -7;
const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

float posx = 0;
float friction = 0.95f;
float velocity = 1;
float crossFireFriction = 0.90f;
float rad = PI / 2;
float omega = 0.025;
int menuOption = 0;
int settingOption = 0;
int length = 6;
bool run = true;
bool startGame = false;
bool startSetting = false;

const char* state[] = {
    "data/image/Main Ship/Main Ship - Base - Very damaged.png",
    "data/image/Main Ship/Main Ship - Base - Damaged.png",
    "data/image/Main Ship/Main Ship - Base - Slight damage.png",
    "data/image/Main Ship/Main Ship - Base - Full health.png"
};

#endif