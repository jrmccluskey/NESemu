#include "cpu.h"

// Just for compilation purposes right now
int main(int argc, char const *argv[]) {
    cpuMemory cpuMem = cpuMemory();
    cpu core = cpu(cpuMem);
    return 0;
}
