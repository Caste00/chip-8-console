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

int main(int argc, char* argv[]) {
    if (argc != 2) 
        throw std::runtime_error("bisogna passare il nome di un file");    

    Chip8 cpu;
    cpu.loadROM(argv[1]);

    if (!initWindow("CHIP-8 Emulator", 640, 320))
        return -1;

    mainLoop(cpu);
    cleanup();

    return 0;
}
