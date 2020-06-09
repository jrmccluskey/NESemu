#include "cpu.h"
#include <cstdlib>

// Just for compilation purposes right now
int main(int argc, char const *argv[]) {
    uint8_t *memory = (uint8_t*) malloc(0x10000);
    cpu core = cpu(memory);
    return 0;
}
