#include "cpu.h"
#include <fstream>
#include <iostream>

void loadROM(Chip8 &cpu, const std::string &filename);

int main() {
    Chip8 cpu;

    loadROM(cpu, "../c8_test.c8");
    // esecuzione del codice
    for (int step = 0; step < 10000; step++) {
        uint16_t opcode = cpu.fetch();
        cpu.execute(opcode);

        std::cout << "Step " << step
                  << " OPCODE=" << std::hex << opcode << "\n";
    }

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