/*******************/
/*     Made by     */
/*     Caste00     */
/*******************/

#pragma once

#include "cpu.h"
#include <SDL2/SDL.h>

#define CHIP8_WIDTH 128
#define CHIP8_HEIGHT 64
#define SCALE 10        // scale 5 for super chip-8 (128x64), scale 10 for chip-8 (64x32)

bool initWindow(const char* title, int width, int height);
void mainLoop(Chip8& cpu);
void cleanup();
void drawChip8Screen(SDL_Renderer* renderer,  Chip8 &cpu);