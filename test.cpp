#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

// Kích thước cửa sổ
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

// Kích thước ô
const int TILE_SIZE = 60;

// Bản đồ ma trận lớn hơn
const int ROWS = 10;
const int COLS = 10;
int matrix[10][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


// Cấu trúc nhân vật
struct Character {
    float x, y;  // Tọa độ hiện tại
    float vx, vy;  // Vận tốc theo trục x và y
    bool isJumping;  // Trạng thái nhảy
};

// Kiểm tra va chạm
bool isValidMove(int matrix[ROWS][COLS], int x, int y) {
    if (x < 0 || y < 0 || x >= ROWS || y >= COLS) {
        return false;
    }
    return matrix[y][x] == 0;
}

// Di chuyển nhân vật
void moveCharacter(Character &character, float deltaTime, int matrix[ROWS][COLS]) {
    // Áp dụng trọng lực
    character.vy += 50.0f * deltaTime;

    float newX = character.x + character.vx * deltaTime;
    float newY = character.y + character.vy * deltaTime;

    int gridX = static_cast<int>(newX / TILE_SIZE);
    int gridY = static_cast<int>(newY / TILE_SIZE);

    if (isValidMove(matrix, gridX, gridY)) {
        character.x = newX;
        character.y = newY;
    } else {
        character.vy = 0;  // Dừng lại khi va chạm
        character.isJumping = false;  // Kết thúc nhảy khi va chạm
    }
}

int main(int argc, char* argv[]) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    // Tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("2D Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Khởi tạo nhân vật
    Character character = {100.0f, 100.0f, 0.0f, 0.0f, false};  // Vị trí ban đầu và vận tốc của nhân vật

    bool quit = false;
    SDL_Event e;
    Uint32 startTime = SDL_GetTicks();

    // Vòng lặp chính
    while (!quit) {
        // Xử lý sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (!character.isJumping) {
                            character.vy = -75.0f;  // Vận tốc nhảy
                            character.isJumping = true;
                        }
                        break;
                    case SDLK_LEFT:
                        character.vx = -200.0f;  // Vận tốc di chuyển trái
                        break;
                    case SDLK_RIGHT:
                        character.vx = 200.0f;  // Vận tốc di chuyển phải
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                        character.vx = 0;  // Dừng di chuyển khi nhả phím
                        break;
                }
            }
        }

        // Tính toán deltaTime
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - startTime) / 1000.0f;
        startTime = currentTime;

        // Di chuyển nhân vật
        moveCharacter(character, deltaTime, matrix);

        // Vẽ màn hình
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Vẽ bản đồ
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                SDL_Rect tileRect = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                if (matrix[i][j] == 1) {
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);  // Màu trắng cho ô giá trị 1
                } else {
                    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);  // Màu đen cho ô giá trị 0
                }
                SDL_RenderFillRect(renderer, &tileRect);
            }
        }

        // Vẽ nhân vật
        SDL_Rect fillRect = {static_cast<int>(character.x), static_cast<int>(character.y), TILE_SIZE, TILE_SIZE};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &fillRect);

        // Cập nhật màn hình
        SDL_RenderPresent(renderer);

        // Đảm bảo tốc độ khung hình 90 FPS
        SDL_Delay(1000 / 90);
    }

    // Dọn dẹp
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
