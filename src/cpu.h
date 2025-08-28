/*******************/
/*     Made by     */
/*     Caste00     */
/*******************/

#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>

#define MEMORY_DIMENSION 0x1000

class Chip8 {
    std::array<uint8_t, MEMORY_DIMENSION> memory;
    std::array<uint8_t, 0x800> video;        // ottimizzabile: un bit per pixel con delle maschere invece che un byte
    std::array<uint16_t, 16> stack;
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
    void write_on_memory(uint16_t addr, uint8_t byte);
    std::array<uint8_t, 0x800> get_video();

private: 
    void push(uint16_t value);
    uint16_t pop();
    uint8_t randByte();
};