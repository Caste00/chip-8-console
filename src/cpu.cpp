/*******************/
/*     Made by     */
/*     Caste00     */
/*******************/

#include "cpu.h"
#include <iostream>

Chip8::Chip8() {
    reset();
}

void Chip8::reset() {
    memory.fill(0);
    stack.fill(0);
    video.fill(0);
    v.fill(0);
    key_state = 0;
    i = 0;
    pc = 0x200;
    sp = 0; 
    delay_timer = 0;
    sound_timer = 0;
    seed = 0xDEADBEEF;
    modernMode = false;

// Fonts standard (80 byte)
    const uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
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

    for (int j = 0; j < 80; j++)
        memory[j] = fontset[j];
}

void Chip8::set_seed(uint32_t seed) {
    this->seed = seed;
}

void Chip8::set_modernMode(bool m) {
    modernMode = m;
}

void Chip8::set_key_state(uint8_t key, bool pressed) {
    if (key > 0xF)  return;
    if (pressed) 
        key_state |= (1 << key);
    else   
        key_state &= ~(1 << key);
}

uint16_t Chip8::fetch() {
    uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
    pc += 2;
    if (pc >= MEMORY_DIMENSION) {
        throw std::runtime_error("PC overflow");
    }

    return opcode;
}

void Chip8::push(uint16_t value) {
    if (sp >= stack.size()) {
        throw std::runtime_error("Stack overflow");
    }
    stack[sp] = value;
    sp++;
}

uint16_t Chip8::pop() {
    if (sp == 0) {
        throw std::runtime_error("Stack underflow");
    }
    sp--;
    return stack[sp];
}

// Invece di avere un seed fisso posso leggere da un pin analogico libero e usare quello come seed, oppure da un timer quando faccio il boot
uint8_t Chip8::randByte() {
    seed = seed * 1664525 + 1013904223;
    return (seed >> 16) & 0xFF;
}

void Chip8::execute(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t nn = opcode & 0x00FF;
    uint16_t nnn = opcode & 0x0FFF;
    switch ((opcode & 0xF000) >> 12) {
        case 0x0: {
            switch (opcode) {
                case 0x00E0: {
                    video.fill(0);
                    break;
                }
                case 0x00EE: {
                    pc = pop();
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case 0x1: {
            pc = nnn;
            break;
        }
        case 0x2: {
            push(pc);
            pc = nnn;
            break;
        }
        case 0x3: {
            if (v[x] == nn)     
                pc += 2;
            break;
        }
        case 0x4: {
            if (v[x] != nn)
                pc += 2;
            break;
        }
        case 0x5: {
            if ((opcode & 0x000F) == 0 && v[x] == v[y])         
                pc += 2;
            break;
        }
        case 0x6: {
            v[x] = nn;
            break;
        }
        case 0x7: {
            v[x] += nn;
            break;
        }
        case 0x8: {
            switch (opcode & 0x000F) {
                case 0: {
                    v[x] = v[y];
                    break;
                }
                case 1: {
                    v[x] |= v[y];
                    break;
                }
                case 2: {
                    v[x] &= v[y];
                    break;
                }
                case 3: {
                    v[x] ^= v[y];
                    break;
                }
                case 4: { 
                    uint16_t sum = v[x] + v[y];
                    v[0xF] = sum >> 8;
                    v[x] = sum & 0xFF;
                    break;
                }
                case 5: {
                    v[0xF] = (v[x] >= v[y]) ? 1 : 0;
                    v[x] -= v[y];
                    break;
                }
                case 6: {
                    v[0xF] = v[x] & 1;      // 0b00000001
                    v[x] >>= 1;
                    break;
                }
                case 7: {
                    v[0xF] = (v[y] >= v[x]) ? 1 : 0;
                    v[x] = v[y] - v[x];
                    break;
                }
                case 0xE: {
                    v[0xF] = (v[x] & 0x80) >> 7;   // 0b10000000 poi sposto il valore al primo bit
                    v[x] <<= 1;
                    break;
                }
                default: 
                    break;
            }
            break;
        }
        case 9: {
            if ((opcode & 0x000F) == 0 && v[x] != v[y]) 
                pc += 2;
            break;
        }
        case 0xA: {
            i = nnn;
            break;
        }
        case 0xB: {
            pc = v[0] + nnn;
            break;
        }
        case 0xC: {
            v[x] = randByte() & nn;
            break;
        }
        case 0xD: {
            v[0xF] = 0;
            uint8_t n = opcode & 0x000F;
            for (int row = 0; row < n; ++row) {
                uint8_t sprite_byte = memory[i + row];
                for (int col = 0; col < 8; ++col) {
                    if (sprite_byte & (0x80 >> col)) {
                        int x_coord = (v[x] + col) % 64;        // il % è per non uscire dal bordo dello schermo
                        int y_coord = (v[y] + row) % 32;
                        int pixel_index = y_coord * 64 + x_coord;

                        if (video[pixel_index] == 1)
                            v[0xF] = 1;

                        video[pixel_index] ^= 1;
                    }
                }
            }
            break;
        }
        case 0xE: {
            switch (nn) {
                case 0x9E: {
                    if (key_state & (1 << v[x]))
                        pc += 2;
                    break;
                }
                case 0xA1: {
                    if (!(key_state & (1 << v[x])))
                        pc += 2;
                    break;
                }
                default: 
                    break;
            }
            break;
        }
        case 0xF: {
            switch (opcode & 0x00FF) {
                case 0x07: {
                    v[x] = delay_timer;
                    break;
                }
                case 0x0A: {
                    if (key_state & (1 << v[x]))
                        break;
                    else
                        pc -= 2;
                    break;
                }
                case 0x15: {
                    delay_timer = v[x];
                    break;
                }
                case 0x18: {
                    sound_timer = v[x];
                    break;
                }
                case 0x1E: {
                    i += v[x];
                    break;
                }
                case 0x29: {
                    i = v[x] * 5;
                    break;
                }
                case 0x33: {
                    memory[i]       = v[x] / 100;
                    memory[i + 1]   = (v[x] / 10) % 10;
                    memory[i + 2]   = v[x] % 10; 
                    break;
                }
                case 0x55: {
                    for (int j = 0; j <= x; j++) {
                        memory[i + j] = v[j]; 
                    }
                    i = modernMode ? i : i + x + 1;
                    break;
                }
                case 0x65: {
                    for (int j = 0; j <= x; j++) {
                        v[j] = memory[i + j];
                    }
                    i = modernMode ? i : i + x + 1;
                    break;
                }
                default: 
                    break;
            }
            break;
        }
        default: 
            break;
    }
}

void Chip8::tickTimer() {
    if (delay_timer > 0)    delay_timer--;
    if (sound_timer > 0)    sound_timer--;
}

void Chip8::write_on_memory(uint16_t addr, uint8_t byte) {
    if (addr >= 0x1000)     throw std::runtime_error("Address to big");
    memory[addr] = byte;
}

std::array<uint8_t, 0x800> Chip8::get_video() {
    return video;
}


// DEBUG
std::array<uint8_t, MEMORY_DIMENSION> Chip8::get_memory() {
    return memory;
}

uint8_t Chip8::get_pc() {
    return pc;
}