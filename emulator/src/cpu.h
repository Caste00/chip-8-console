/*******************/
/*     Made by     */
/*     Caste00     */
/*******************/

#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>

#define HEIGHT 64
#define WIDTH 128
#define MEMORY_DIMENSION 0x1000
const int VIDEO_BUFFER_DIMENSION = WIDTH * HEIGHT / 8;

class Chip8 {
    std::array<uint8_t, MEMORY_DIMENSION> memory;
    std::array<uint8_t, VIDEO_BUFFER_DIMENSION> video;
    std::array<uint16_t, 32> stack;
    std::array<uint8_t, 16> v;
    uint16_t key_state;
    uint16_t i;
    uint16_t pc;
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint32_t seed;
    bool modernMode;

public: 
    Chip8();
    void reset();
    uint16_t fetch();
    void execute(uint16_t opcode);
    void tickTimer();
    void set_seed(uint32_t seed);
    void set_modernMode(bool m);
    void set_key_state(uint8_t key, bool pressed);
    void write_on_memory(uint16_t addr, uint8_t byte);
    std::array<uint8_t, VIDEO_BUFFER_DIMENSION> get_video();
    
    private: 
    void push(uint16_t value);
    uint16_t pop();
    uint8_t randByte();
    
    // Per debug
    std::array<uint8_t, MEMORY_DIMENSION> get_memory();
    uint8_t get_pc();
};