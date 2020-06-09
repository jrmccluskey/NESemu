#include <cstdint>
#include "memory.h"

class cpu {
private:
    // Registers
    uint8_t accumulator;
    uint8_t regX;
    uint8_t regY;
    // Control Flow
    uint16_t pc;
    uint8_t sp;
    // Bit flags
    bool carry;
    bool zero;
    bool interruptDisable;
    bool decimalMode;
    bool breakCom;
    bool overflow;
    bool negative;
    // Memory Reference
    cpuMemory memory;

public:
    cpu(cpuMemory memory) {
        this->memory = memory;
        pc = 0x0000;
        sp = 0xff;
    }
};
