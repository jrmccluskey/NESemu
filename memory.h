#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>

class cpuMemory{
private:
    std::vector<uint8_t> mem;

public:
    cpuMemory() {
        mem.reserve(8192);
    }

    // Remember that you're pulling memory addresses but the
    // representation in code is in an array so you have to convert
    inline int calcIndex(uint8_t lowBits, uint8_t hiBits) {
        uint16_t addr = ((hiBits << 8) | lowBits);
        int index = addr/8;
        if(index > 8191) {
            // Out-of-bounds memory accesses wrap around
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

    uint8_t readIndex(int index) {
        return mem[index];
    }

    void writeIndex(int index, uint8_t val) {
        std::cout << index << ": " << val << "\n";
        mem[index] = val;
    }
};
