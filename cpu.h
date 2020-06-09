#include <cstdint>

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
    uint8_t *memory;

public:
    cpu(uint8_t *memory) {
        this->memory = memory;
    }
};
