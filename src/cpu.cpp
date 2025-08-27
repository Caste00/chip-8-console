#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>

using namespace std;

class Chip8 {
    array<uint8_t, 0x1000> memory;
    array<uint8_t, 0x800> video;        // ottimizzabile: un bit per pixel con delle maschere invece che un byte
    array<uint16_t, 16> stack;
    array<uint8_t, 16> v;
    uint16_t i;
    uint16_t pc;
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;

public: 
    Chip8();
    void reset();
    uint16_t fetch();
    void execute(uint16_t opcode);

private: 
    void push(uint16_t value);
    uint16_t pop();
};

Chip8::Chip8() {
    reset();
}

void Chip8::reset() {
    memory.fill(0);
    stack.fill(0);
    v.fill(0);
    i = 0;
    pc = 0x200;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;

// Fonts standard (80 byte)
    const uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (int i = 0; i < 80; i++)
        memory[i] = fontset[i];
}

uint16_t Chip8::fetch() {
    uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
    pc += 2;
    return opcode;
}

void Chip8::push(uint16_t value) {
    if (sp >= stack.size()) {
        throw runtime_error("Stack overflow");
    }
    stack[sp] = value;
    sp++;
}

uint16_t Chip8::pop() {
    if (sp <= 0) {
        throw runtime_error("Stack overflow");
    }
    sp--;
    return stack[sp];
}

void Chip8::execute(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0);
    uint8_t nn = opcode & 0x00FF;
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                case 0x00E0: 
                    video.fill(0);
                    break;
                case 0x00EE: 
                    pc = pop();
                    break;
                default:
                    break;
            }
            break;
        case 0x1000:
            pc = opcode & 0x0FFF;
            break;
        case 0x2000:
            push(pc);
            pc = opcode & 0x0FFF;
            break;
        case 0x3000:
            if (v[x] == nn)     
                pc += 2;
            break;
        case 0x4000:
            if (v[x] != nn)
                pc += 2;
            break;
        case 0x5000:
            if (opcode & 0x000F == 0) {
                if (v[x] == v[y])       
                    pc += 2;
            }
            break;
        case 0x6000:
            v[x] = nn;
            break;
        case 0x7000:
            v[x] += nn;
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0:
                    v[x] = v[y];
                    break;
                case 1:
                    v[x] |= v[y];
                    break;
                case 2:
                    v[x] &= v[y];
                    break;
                case 3: 
                    V[x] t
                default: 
                    break;
            }
    }
}