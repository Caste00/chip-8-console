/*******************/
/*     Made by     */
/*     Caste00     */
/*******************/

#include "cpu.h"
#include "display.h"
#include <fstream>
#include <iostream>
#include <iomanip>

void loadROM(Chip8 &cpu, const std::string &filename);

int main() {
    Chip8 cpu;

    loadROM(cpu, "../../assembler/prova.ch8");

    if (!initWindow("CHIP-8 Emulator", 640, 320))
        return -1;

    mainLoop(cpu);
    cleanup();

    return 0;
}

void loadROM(Chip8 &cpu, const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file)  throw std::runtime_error("Impossibile aprire il file");

    uint8_t byte;
    uint16_t addr = 0x50;

    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        cpu.write_on_memory(addr++, byte);
    }
}
