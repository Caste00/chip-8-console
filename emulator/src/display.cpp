/*******************/
/*     Made by     */
/*     Caste00     */
/*******************/

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

    const int TIMER_INTERVAL_MS = 1000 / 60;
    uint32_t lastTimerUpdate = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)     
                running = false;
            else if (e.type == SDL_KEYDOWN && !e.key.repeat) {
                switch (e.key.keysym.sym) {
                    case SDLK_1: 
                        cpu.set_key_state(0x1, true);
                        break;
                    case SDLK_2:
                        cpu.set_key_state(0x2, true);
                        break;
                    case SDLK_3:
                        cpu.set_key_state(0x3, true);
                        break;
                    case SDLK_4:
                        cpu.set_key_state(0x4, true);
                        break;
                    case SDLK_5:
                        cpu.set_key_state(0x5, true);
                        break;
                    case SDLK_6:
                        cpu.set_key_state(0x6, true);
                        break;
                    case SDLK_7:
                        cpu.set_key_state(0x7, true);
                        break;
                    case SDLK_8:
                        cpu.set_key_state(0x8, true);
                        break;
                    case SDLK_9:
                        cpu.set_key_state(0x9, true);
                        break;
                    case SDLK_a:
                        cpu.set_key_state(0xA, true);
                        break;
                    case SDLK_b:
                        cpu.set_key_state(0xB, true);
                        break;
                    case SDLK_c:
                        cpu.set_key_state(0xC, true);
                        break;
                    case SDLK_d:
                        cpu.set_key_state(0xD, true);
                        break;
                    case SDLK_e:
                        cpu.set_key_state(0xE, true);
                        break;
                    case SDLK_f:
                        cpu.set_key_state(0xF, true);
                        break;
                    default: 
                        break;
                }
            }        
            else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_1: 
                        cpu.set_key_state(0x1, false);
                        break;
                    case SDLK_2:
                        cpu.set_key_state(0x2, false);
                        break;
                    case SDLK_3:
                        cpu.set_key_state(0x3, false);
                        break;
                    case SDLK_4:
                        cpu.set_key_state(0x4, false);
                        break;
                    case SDLK_5:
                        cpu.set_key_state(0x5, false);
                        break;
                    case SDLK_6:
                        cpu.set_key_state(0x6, false);
                        break;
                    case SDLK_7:
                        cpu.set_key_state(0x7, false);
                        break;
                    case SDLK_8:
                        cpu.set_key_state(0x8, false);
                        break;
                    case SDLK_9:
                        cpu.set_key_state(0x9, false);
                        break;
                    case SDLK_a:
                        cpu.set_key_state(0xA, false);
                        break;
                    case SDLK_b:
                        cpu.set_key_state(0xB, false);
                        break;
                    case SDLK_c:
                        cpu.set_key_state(0xC, false);
                        break;
                    case SDLK_d:
                        cpu.set_key_state(0xD, false);
                        break;
                    case SDLK_e:
                        cpu.set_key_state(0xE, false);
                        break;
                    case SDLK_f:
                        cpu.set_key_state(0xF, false);
                        break;
                    default:
                        break;
                }
            }
        }

        // cpu cycle, eseguo 10 cicli di cpu per ogni frame
        for (int i = 0; i < 10; i++) {
            //std::cout << std::hex << cpu.get_pc() << " ";
            uint16_t opcode = cpu.fetch();
            cpu.execute(opcode);
        }

        // aggiorno il timer a 60 Hz
        uint32_t now = SDL_GetTicks();
        if (now - lastTimerUpdate >= TIMER_INTERVAL_MS) {
            cpu.tickTimer();
            lastTimerUpdate = now;
        }

        drawChip8Screen(renderer, cpu);

        // piccola pausa per non saturare la cpu
        SDL_Delay(1);
    }
}

void drawChip8Screen(SDL_Renderer* renderer, Chip8& cpu) {
    const auto& buffer = cpu.get_video();
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);         // sfondo nero
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // pixel bianchi
    
    for (int y = 0; y < CHIP8_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_WIDTH; x++) {
            int byteIndex = y * (CHIP8_WIDTH / 8) + (x / 8);
            uint8_t mask = 0x80 >> (x % 8);

            if (buffer[byteIndex] & mask) {
                SDL_Rect rect { x * SCALE, y * SCALE, SCALE, SCALE};
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