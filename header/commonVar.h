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
#include <vector>
#include <windows.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const int BPP = 32;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Renderer* backgroundRenderer = NULL;
static SDL_Texture* texture = NULL;
static SDL_Surface* surface = NULL;
SDL_Event e;

SDL_Surface* playerSurface = NULL;
SDL_Texture* playerTexture = NULL;

SDL_Surface* rocketSurface = NULL;
SDL_Texture* rocketTexture = NULL;

SDL_Surface* raySurface = NULL;
SDL_Texture* rayTexture = NULL;

SDL_Surface* destroySurface = NULL;
SDL_Texture* destroyTexture = NULL;

SDL_Surface* backgroundSurface[3] = {NULL};
SDL_Texture* backgroundTexture[3] = {NULL};

SDL_Surface* modeSurface = NULL;
SDL_Texture* modeTexture = NULL;

const float PI = 3.14159; 
const int FPS = 90;
const int FRAME_DELAY = 1000 / FPS;
const int MAX_VELOCITY = 2;
const int MIN_VELOCITY = -2;

float layer1 = 0;
float layer2 = 0;
float layer3 = 0;
float friction = 0.99f;
float velocity = 1;
float crossFireFriction = 0.99f;
float rad = PI / 2;
float omega = 0.025;
float sensitivity = 150;

int menuOption = 0;
int settingOption = 0;
int length = 6;
int volume = 10;
int move = 1;
int maxBullet = 4;
int score = 0;
int gameMode = 1;
int highScore = 0;

bool run = true;
bool startGame = false;
bool startSetting = false;
bool startHighScore = false;
bool play = true;

const char* state[] = {
    "data/image/Main Ship/Main Ship - Base - Very damaged.png",
    "data/image/Main Ship/Main Ship - Base - Damaged.png",
    "data/image/Main Ship/Main Ship - Base - Slight damage.png",
    "data/image/Main Ship/Main Ship - Base - Full health.png"
};

const char* backgroundPath[] = {
    "data/image/Backgrounds/Condesed/Starry background  - Layer 01 - Void.png",
    "data/image/BackGrounds/Condesed/Starry background  - Layer 02 - Stars.png",
    "data/image/BackGrounds/Condesed/Starry background  - Layer 03 - Stars.png"
};

const char* rocketPath = "data/image/Main Ship/Projectile/Main Ship - Rocket.png";
const char* rayPath = "data/image/Main Ship/Projectile/Main Ship - Ray.png";
const char* destroyPath = "data/image/Enemy/Basic/Destruction/Nairan - Fighter -  Destruction.png";

const char* config = "data/config/gameConfig.json";
const char* scorePath = "data/highScore.json";

#endif