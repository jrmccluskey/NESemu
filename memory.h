#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>

const int MEMORY_SIZE = 65536;

class cpuMemory{
private:
    std::vector<uint8_t> mem;

public:
    cpuMemory() {
        mem.reserve(MEMORY_SIZE);
    }

    // Helper to convert the little-endian rep to an address
    inline uint16_t calcAddr(uint8_t lowBits, uint8_t hiBits) {
        return ((hiBits << 8) | lowBits);
    }

    uint8_t readAddress(uint8_t lowBits, uint8_t hiBits) {
        uint16_t addr = calcAddr(lowBits, hiBits);
        return mem[addr];
    }

    // Overloaded function for PC addresses
    uint8_t readAddress(uint16_t addr) {
        return mem[addr];
    }

    void writeAddress(uint8_t lowBits, uint8_t hiBits, uint8_t val) {
        uint16_t addr = calcAddr(lowBits, hiBits);
        mem[addr] = val;
    }

    // Overloaded function for PC addresses
    void writeAddress(uint16_t addr, uint8_t val) {
        mem[addr] = val;
    }
};
