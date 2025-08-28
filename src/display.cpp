#include "display.h"
#include "cpu.h"
#include <iostream>

using namespace std;

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;


bool initWindow(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Errore SDL: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow(title, 
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_SHOWN);

    if (!window) {
        cerr << "Errore creazione finestra: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Errore creazione renderer" << SDL_GetError() << endl;
        return false;
    }

    return true;
}

void mainLoop(Chip8& cpu) {
    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)     running = false;
        }

        // cpu cycle
        uint16_t opcode = cpu.fetch();
        cpu.execute(opcode);
    
        cpu.tickTimer();
        drawChip8Screen(renderer, cpu);

        // Limita gli FPS a circa 60
        SDL_Delay(16);
    }
}

void drawChip8Screen(SDL_Renderer* renderer, Chip8& cpu) {
    const auto& buffer = cpu.get_video();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);         // sfondo nero
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // pixel bianchi

    for (int y = 0; y < CHIP8_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_WIDTH; x++) {
            if (buffer[y * CHIP8_WIDTH + x]) {
                SDL_Rect rect;
                rect.x = x * SCALE;
                rect.y = y * SCALE;
                rect.w = SCALE;
                rect.h = SCALE;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}