#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 20;
const int GRID_WIDTH = SCREEN_WIDTH / GRID_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / GRID_SIZE;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct SnakeSegment {
    int x, y;
    SnakeSegment(int x_, int y_) : x(x_), y(y_) {}
};

class SnakeGame {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SnakeSegment> snake;
    Direction direction;
    int foodX, foodY;
    bool running;
    int score;

    void spawnFood() {
        foodX = rand() % GRID_WIDTH;
        foodY = rand() % GRID_HEIGHT;
    }

    bool checkCollision() {
        // Check wall collision
        if (snake[0].x < 0 || snake[0].x >= GRID_WIDTH ||
            snake[0].y < 0 || snake[0].y >= GRID_HEIGHT) {
            return true;
        }
        // Check self collision
        for (size_t i = 1; i < snake.size(); i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                return true;
            }
        }
        return false;
    }

public:
    SnakeGame() {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, 
                                 SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        
        // Initialize snake
        snake.push_back(SnakeSegment(GRID_WIDTH/2, GRID_HEIGHT/2));
        direction = RIGHT;
        running = true;
        score = 0;
        srand(time(0));
        spawnFood();
    }

    ~SnakeGame() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void handleInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: if (direction != DOWN) direction = UP; break;
                    case SDLK_DOWN: if (direction != UP) direction = DOWN; break;
                    case SDLK_LEFT: if (direction != RIGHT) direction = LEFT; break;
                    case SDLK_RIGHT: if (direction != LEFT) direction = RIGHT; break;
                }
            }
        }
    }

    void update() {
        // Store previous positions
        std::vector<SnakeSegment> prevSnake = snake;
        
        // Move head
        switch (direction) {
            case UP: snake[0].y--; break;
            case DOWN: snake[0].y++; break;
            case LEFT: snake[0].x--; break;
            case RIGHT: snake[0].x++; break;
        }

        // Move body
        for (size_t i = 1; i < snake.size(); i++) {
            snake[i] = prevSnake[i-1];
        }

        // Check food collision
        if (snake[0].x == foodX && snake[0].y == foodY) {
            snake.push_back(prevSnake.back());
            spawnFood();
            score += 10;
        }

        if (checkCollision()) {
            running = false;
        }
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw snake
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (const auto& segment : snake) {
            SDL_Rect rect = {segment.x * GRID_SIZE, segment.y * GRID_SIZE, 
                           GRID_SIZE-2, GRID_SIZE-2};
            SDL_RenderFillRect(renderer, &rect);
        }

        // Draw food
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect foodRect = {foodX * GRID_SIZE, foodY * GRID_SIZE, 
                           GRID_SIZE-2, GRID_SIZE-2};
        SDL_RenderFillRect(renderer, &foodRect);

        SDL_RenderPresent(renderer);
    }

    void run() {
        while (running) {
            handleInput();
            update();
            render();
            SDL_Delay(100); // Control game speed
        }
    }
};

int main(int argc, char* argv[]) {
    SnakeGame game;
    game.run();
    return 0;
}