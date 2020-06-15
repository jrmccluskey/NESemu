#include "cpu.h"
#include <iostream>

// Just for compilation purposes right now
int main(int argc, char const *argv[]) {
    cpuMemory cpuMem = cpuMemory();
    cpu core = cpu(cpuMem);
    core.emulateOp();
    return 0;
}
