#include <cstdint>
#include <cstdlib>
#include <cstdio>

class cpuMemory{
private:
    uint8_t *mem;

public:
    cpuMemory() {
        this->mem = (uint8_t*) malloc(0x10000);
        mem[31] = 42;
    }

    inline int calcIndex(uint8_t lowBits, uint8_t hiBits) {
        uint16_t addr = ((hiBits << 8) | lowBits);
        int index = addr/8;
        if(index > 8191) {
            index = index % 8192;
        }
        return index;
    }

    uint8_t readAddress(uint8_t lowBits, uint8_t hiBits) {
        int index = calcIndex(lowBits, hiBits);
        return mem[index];
    }

    void writeAddress(uint8_t lowBits, uint8_t hiBits, uint8_t val) {
        int index = calcIndex(lowBits, hiBits);
        mem[index] = val;
    }
};
