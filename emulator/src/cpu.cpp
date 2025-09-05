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
    schipMode = false;
    running = true;

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

    const uint8_t fontset10[160] = {
        0xF0, 0xF0, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xF0, 0xF0, // 0
        0x20, 0x20, 0x60, 0x60, 0x20, 0x20, 0x20, 0x20, 0x70, 0x70, // 1
        0xF0, 0xF0, 0x10, 0x10, 0xF0, 0xF0, 0x80, 0x80, 0xF0, 0xF0, // 2
        0xF0, 0xF0, 0x10, 0x10, 0xF0, 0xF0, 0x10, 0x10, 0xF0, 0xF0, // 3
        0x90, 0x90, 0x90, 0x90, 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, // 4
        0xF0, 0xF0, 0x80, 0x80, 0xF0, 0xF0, 0x10, 0x10, 0xF0, 0xF0, // 5
        0xF0, 0xF0, 0x80, 0x80, 0xF0, 0xF0, 0x90, 0x90, 0xF0, 0xF0, // 6
        0xF0, 0xF0, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x40, 0x40, // 7
        0xF0, 0xF0, 0x90, 0x90, 0xF0, 0xF0, 0x90, 0x90, 0xF0, 0xF0, // 8
        0xF0, 0xF0, 0x90, 0x90, 0xF0, 0xF0, 0x10, 0x10, 0xF0, 0xF0, // 9
        0xF0, 0xF0, 0x90, 0x90, 0xF0, 0xF0, 0x90, 0x90, 0x90, 0x90, // A
        0xE0, 0xE0, 0x90, 0x90, 0xE0, 0xE0, 0x90, 0x90, 0xE0, 0xE0, // B
        0xF0, 0xF0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xF0, 0xF0, // C
        0xE0, 0xE0, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE0, 0xE0, // D
        0xF0, 0xF0, 0x80, 0x80, 0xF0, 0xF0, 0x80, 0x80, 0xF0, 0xF0, // E
        0xF0, 0xF0, 0x80, 0x80, 0xF0, 0xF0, 0x80, 0x80, 0x80, 0x80  // F
    };

    std::copy(std::begin(fontset), std::end(fontset), memory.begin());
    std::copy(std::begin(fontset10), std::end(fontset10), memory.begin() + 80);
}

void Chip8::set_modernMode(bool m) {
    schipMode = m;
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

uint8_t Chip8::randByte() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, 0xFF);

    return static_cast<uint8_t>(dist(gen));
}

void Chip8::execute(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t nn = opcode & 0x00FF;
    uint16_t nnn = opcode & 0x0FFF;
    switch ((opcode & 0xF000) >> 12) {
        case 0x0: {
            switch (opcode & 0x00FF) {
                case 0xE0: {
                    video.fill(0);
                    break;
                }
                case 0xEE: {
                    pc = pop();
                    break;
                }
                case 0xFB: {
                    if (schipMode) {
                        for (int i = 0; i < HEIGHT; i++) {
                            for (int j = ROW_BYTE; j >= 0; j++) {
                                int index = i * ROW_BYTE + j;
                                uint8_t current = video[index];
                                uint8_t next = (j > 0) ? video[index - 1] : 0;

                                video[index] = (current >> 4) | (next << 4);
                            }
                        }
                    }
                    break;
                }
                case 0xFC: {
                    if (schipMode) {
                        for (int i = 0; i < HEIGHT; i++) {
                            for (int j = 0; j < ROW_BYTE; j++) {
                                int index = i * ROW_BYTE + j;
                                uint8_t current = video[index];
                                uint8_t next = (j + 1 < ROW_BYTE) ? video[index + 1] : 0;
                                
                                video[index] = (current << 4) | (next >> 4);
                            }
                        }
                    }
                    break;
                } 
                case 0xFD: {
                    if (schipMode)
                        running = false;
                    break;
                }
                case 0xFE: {
                    if (schipMode)
                        schipMode = false;
                    break;
                }
                case 0xFF: {
                    if (!schipMode) 
                        schipMode = true;
                    break;
                }
                default: {
                    if ((opcode & 0xF0) == 0xC0 && schipMode) {        // caso 00CN
                        uint8_t n = opcode & 0x000F;
                        int offset = n * ROW_BYTE;
                        for (int i = VIDEO_BUFFER_DIMENSION; i >= 0; i++) {
                            if (i >= offset)
                                video[i] = video[i + offset];
                            else 
                                video[i] = 0x00;
                        }
                    }
                    break;
                }
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

            if (schipMode && n == 0) {
                for (int row = 0; row < 16; ++row) {
                    uint16_t sprite_row = (memory[i + row * 2] << 8) | memory[i + row * 2 + 1];
                    int y_coord = (v[y] + row) % HEIGHT;

                    for (int col = 0; col < 16; col++) {
                        if (sprite_row & (0x8000 >> col)) {
                            int x_coord = (v[x] + col) % WIDTH;

                            int byte_index = y_coord * (WIDTH / 8) + (x_coord / 8);
                            uint8_t mask = 0x80 >> (x_coord % 8);

                            if (video[byte_index] & mask)
                                v[0xF] = 1;

                            video[byte_index] ^= mask;
                        }
                    }
                }
            } else {
                // Sprite standard 8xN
                for (int row = 0; row < n; ++row) {
                    uint8_t sprite_byte = memory[i + row];
                    int y_coord = (v[y] + row) % HEIGHT;

                    for (int col = 0; col < 8; col++) {
                        if (sprite_byte & (0x80 >> col)) {
                            int x_coord = (v[x] + col) % WIDTH;

                            int byte_index = y_coord * (WIDTH / 8) + (x_coord / 8);
                            uint8_t mask = 0x80 >> (x_coord % 8);

                            if (video[byte_index] & mask)
                                v[0xF] = 1;

                            video[byte_index] ^= mask;
                        }
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
                case 0x30: {
                    if (schipMode)
                        i = 80 + v[x] * 10;
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
                    i = schipMode ? i : i + 1;
                    break;
                }
                case 0x65: {
                    for (int j = 0; j <= x; j++) {
                        v[j] = memory[i + j];
                    }
                    i = schipMode ? i : i + 1;
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
    if (addr >= 0x1000)     throw std::runtime_error("Address too big");
    memory[addr] = byte;
}

std::array<uint8_t, VIDEO_BUFFER_DIMENSION> Chip8::get_video() {
    return video;
}

void Chip8::loadROM(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file)  
        throw std::runtime_error("file error");

    uint8_t byte;
    uint16_t addr = pc;

    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        if (addr >= MEMORY_DIMENSION)
            throw std::runtime_error("ROM execede memory");
        write_on_memory(addr++, byte);
    }
}

bool Chip8::is_schipMode() {
    return schipMode;
}