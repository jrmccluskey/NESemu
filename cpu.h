#include <cstdint>
#include <iostream>
#include "memory.h"

const uint8_t CARRY_MASK = 0x01;
const uint8_t ZERO_MASK = 0x02;
const uint8_t INTERRUPT_MASK = 0x04;
const uint8_t DECIMAL_MASK = 0x08;
const uint8_t BREAK_MASK = 0x10;
const uint8_t OVERFLOW_MASK = 0x40;
const uint8_t NEGATIVE_MASK = 0x80;

const uint8_t STACK_TOP = 0xff;

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
    uint8_t ps;
    // Memory Reference
    cpuMemory memory;

public:
    cpu(cpuMemory memory) {
        accumulator = 0x00;
        regX = 0x00;
        regY = 0x00;
        this->memory = memory;
        pc = 0x0000;
        sp = STACK_TOP;
        ps = 0x34;
    }

    inline void setNegative(uint8_t value) {
        if((value & NEGATIVE_MASK) != 0x00) {
            this->ps = this->ps | NEGATIVE_MASK;
        } else {
            this->ps = this->ps | (~NEGATIVE_MASK);
        }
    }

    inline void setZero(uint8_t value) {
        if(value == 0x00) {
            this->ps = this->ps | ZERO_MASK;
        } else {
            this->ps = this->ps | (~ZERO_MASK);
        }
    }

    uint8_t readZeroPage(uint8_t offset) {
        uint8_t loBits = this->memory.readAddress(this->pc + 0x01);
        loBits = (loBits + offset) % 256;
        return this->memory.readAddress(loBits, 0x00);
    }

    void writeZeroPage(uint8_t value, uint8_t offset) {
        uint8_t loBits = this->memory.readAddress(this->pc + 0x01);
        loBits = (loBits + offset) % 256;
        this->memory.writeAddress(loBits, 0x00, value);
    }

    uint8_t readAbsolute(uint8_t offset) {
        uint8_t loBits = this->memory.readAddress(this->pc + 0x01);
        uint8_t hiBits = this->memory.readAddress(this->pc + 0x02);
        return ((hiBits << 8) | loBits) + offset;
    }

    void writeAbsolute(uint8_t value, uint8_t offset) {
        uint8_t loBits = this->memory.readAddress(this->pc + 0x01);
        uint8_t hiBits = this->memory.readAddress(this->pc + 0x02);
        uint16_t addr = ((hiBits << 8) | loBits) + offset;
        this->memory.writeAddress(addr, value);
    }

    uint16_t branchCalc(uint16_t progCounter, uint8_t offset) {
        uint8_t maskedOffset = offset & (~NEGATIVE_MASK);
        if(offset == maskedOffset) {
            return progCounter + maskedOffset;
        } else {
            return progCounter - maskedOffset;
        }
    }

    inline void branchSet(uint8_t mask) {
        if((this->ps & mask) != 0x00) {
            uint8_t offset = this->memory.readAddress(this->pc + 0x01);
            this->pc = branchCalc(this->pc, offset);
        }
        this->pc += 0x02;
    }

    inline void branchClear(uint8_t mask) {
        if((this->ps & mask) == 0x00) {
            uint8_t offset = this->memory.readAddress(this->pc + 0x01);
            this->pc = branchCalc(this->pc, offset);
        }
        this->pc += 0x02;
    }

    inline void push(uint8_t value) {
        this->memory.writeAddress(this->sp, 0x01, value);
        if(this->pc == 0x00) {
            this->pc = STACK_TOP;
        } else {
            this->pc -= 0x01;
        }
    }

    inline uint8_t pop() {
        if(this->pc == STACK_TOP) {
            this->pc = 0x00;
        } else {
            this->pc += 0x01;
        }
        uint8_t value = this->memory.readAddress(this->sp, 0x01);
        return value;
    }

    void emulateOp() {
        uint8_t opCode = this->memory.readAddress(pc);
        uint8_t loBits;
        uint8_t hiBits;
        uint16_t addrScratch;
        switch(opCode){
            // ADC - Add Memory to Accumulator with Carry
            case 0x69:
            case 0x65:
            case 0x75:
            case 0x6d:
            case 0x7d:
            case 0x79:
            case 0x61:
            case 0x71:
                std::cout << "ADC\n";
                break;
            // AND - AND Memory with Accumulator
            case 0x29:
            case 0x25:
            case 0x35:
            case 0x2d:
            case 0x3d:
            case 0x39:
            case 0x21:
            case 0x31:
                std::cout << "AND\n";
                break;
            // ASL - Shift Left One Bit
            case 0x0a:
            case 0x06:
            case 0x16:
            case 0x0e:
            case 0x1e:
                std::cout << "ASL\n";
                break;
            // BCC - Branch on Carry Clear
            case 0x90:
                branchClear(CARRY_MASK);
                break;
            // BCS - Branch on Carry Set
            case 0xb0:
                branchSet(CARRY_MASK);
                break;
            // BEQ - Branch on Result Zero
            case 0xf0:
                branchSet(ZERO_MASK);
                break;
            // BIT - Test Bits in Memory with Accumulator
            case 0x24:
            case 0x2c:
                std::cout << "BIT\n";
                break;
            // BMI - Branch on Result Minus
            case 0x30:
                branchSet(NEGATIVE_MASK);
                break;
            // BNE - Branch on Result not Zero
            case 0xd0:
                branchClear(ZERO_MASK);
                break;
            // BPL - Branch on Result Plus
            case 0x10:
                branchClear(NEGATIVE_MASK);
                break;
            // BRK - Force Break
            case 0x00:
                std::cout << "BRK\n";
                break;
            // BVC - Branch on Overflow Clear
            case 0x50:
                branchClear(OVERFLOW_MASK);
                break;
            // BVS - Branch on Overflow Set
            case 0x70:
                branchSet(OVERFLOW_MASK);
                break;
            // CLC - Clear Carry Flag
            case 0x18:
                this->ps = this->ps & (~CARRY_MASK);
                this->pc += 0x01;
                break;
            // CLD - Clear Decimal Mode
            case 0xd8:
                this->ps = this->ps & (~DECIMAL_MASK);
                this->pc += 0x01;
                break;
            // CLI - Clear Interrupt Disable Bit
            case 0x58:
                this->ps = this->ps & (~INTERRUPT_MASK);;
                this->pc += 0x01;
                break;
            // CLV - Clear Overflow Flag
            case 0xb8:
                this->ps = this->ps & (~OVERFLOW_MASK);
                this->pc += 0x01;
                break;
            // CMP - Compare Memory with Accumulator
            case 0xc9:
            case 0xc5:
            case 0xd5:
            case 0xcd:
            case 0xdd:
            case 0xd9:
            case 0xc1:
            case 0xd1:
                std::cout << "CMP\n";
                break;
            // CPX - Compare Memory and Index X
            case 0xe0:
            case 0xe4:
            case 0xec:
                std::cout << "CPX\n";
                break;
            // CPY - Compare Memory and Index Y
            case 0xc0:
            case 0xc4:
            case 0xcc:
                std::cout << "CPY\n";
                break;
            // DEC - Decrement Memory by One
            case 0xc6:
            case 0xd6:
            case 0xce:
            case 0xde:
                std::cout << "DEC\n";
                break;
            // DEX - Decrement Index X by One
            case 0xca:
                this->regX -= 1;
                setNegative(this->regX);
                setZero(this->regX);
                this->pc += 0x01;
                break;
            // DEY - Decrement Index Y by One
            case 0x88:
                this->regY -= 1;
                setNegative(this->regY);
                setZero(this->regY);
                this->pc += 0x01;
                break;
            // EOR - Exclusive-OR Memory with Accumulator
            case 0x49:
            case 0x45:
            case 0x55:
            case 0x4d:
            case 0x5d:
            case 0x59:
            case 0x41:
            case 0x51:
                std::cout << "EOR\n";
                break;
            // INC - Increment Memory by One
            case 0xe6:
            case 0xf6:
            case 0xee:
            case 0xfe:
                std::cout << "INC\n";
                break;
            // INX - Increment Index X by One
            case 0xe8:
                std::cout << "INX\n";
                this->pc += 0x01;
                break;
            // INY - Increment Index Y by One
            case 0xc8:
                std::cout << "INY\n";
                this->pc += 0x01;
                break;
            // JMP - Jump to New Location
            case 0x4c:
                loBits = this->memory.readAddress(this->pc + 0x01);
                hiBits = this->memory.readAddress(this->pc + 0x02);
                this->pc = ((hiBits << 8) | loBits);
                break;
            case 0x6c:
                std::cout << "JMP\n";
                break;
            // JSR - Jump to New Location Saving Return Address
            case 0x20:
                std::cout << "JSR\n";
                break;
            // LDA - Load Accumulator with Memory
            case 0xa9:
            case 0xa5:
            case 0xb5:
            case 0xad:
            case 0xbd:
            case 0xb9:
            case 0xa1:
            case 0xb1:
                std::cout << "LDA\n";
                break;
            // LDX - Load Index X with Memory
            case 0xa2:
                this->regX = this->memory.readAddress(this->pc + 0x01);
                setNegative(this->regX);
                setZero(this->regX);
                this->pc += 0x02;
                break;
            case 0xa6:
            case 0xb6:
            case 0xae:
            case 0xbe:
                std::cout << "LDX\n";
            // LDY - Load Index Y with Memory
            case 0xa0:
                this->regY = this->memory.readAddress(this->pc + 0x01);
                setNegative(this->regY);
                setZero(this->regY);
                this->pc += 0x02;
                break;
            case 0xa4:
            case 0xb4:
            case 0xac:
            case 0xbc:
                std::cout << "LDY\n";
                break;
            // LSR - Shift One Bit Right
            case 0x4a:
            case 0x46:
            case 0x56:
            case 0x4e:
            case 0x5e:
                std::cout << "LSR\n";
                break;
            // NOP - No Operation
            case 0xea:
                this->pc += 0x01;
                break;
            // ORA - OR Memory with Accumulator
            case 0x09:
            case 0x05:
            case 0x15:
            case 0x0d:
            case 0x1d:
            case 0x19:
            case 0x01:
            case 0x11:
                std::cout << "ORA\n";
                break;
            // PHA - Push Accumulator on Stack
            case 0x48:
                push(this->accumulator);
                this->pc += 0x01;
                break;
            // PHP - Push Processor Status on Stack
            case 0x08:
                push(this->ps);
                this->pc += 0x01;
                break;
            // PLA - Pull Accumulator from Stack
            case 0x68:
                this->accumulator = pop();
                setNegative(this->accumulator);
                setZero(this->accumulator);
                this->pc += 0x01;
                break;
            // PLP - Pull Processor Status from Stack
            case 0x28:
                this->ps = pop();
                this->pc += 0x01;
                break;
            // ROL - Rotate One Bit Left
            case 0x2a:
            case 0x26:
            case 0x36:
            case 0x2e:
            case 0x3e:
                std::cout << "ROL\n";
                break;
            // ROR - Rotate One Bit Right
            case 0x6a:
            case 0x66:
            case 0x76:
            case 0x6e:
            case 0x7e:
                std::cout << "ROR\n";
                break;
            // RTI - Return from Interrupt
            case 0x40:
                std::cout << "RTI\n";
                this->pc += 0x01;
                break;
            // RTS - Return from Subroutine
            case 0x60:
                std::cout << "RTS\n";
                this->pc += 0x01;
                break;
            // SBC - Substract Memory from Accumulator with Borrow
            case 0xe9:
            case 0xe5:
            case 0xf5:
            case 0xed:
            case 0xfd:
            case 0xf9:
            case 0xe1:
            case 0xf1:
                std::cout << "SBC\n";
                break;
            // SEC - Set Carry Flag
            case 0x38:
                this->ps = this->ps | CARRY_MASK;
                this->pc += 0x01;
                break;
            // SED - Set Decimal Flag
            case 0xf8:
                this->ps = this->ps | DECIMAL_MASK;
                this->pc += 0x01;
                break;
            // SEI - Set Interrupt Disable Status
            case 0x78:
                this->ps = this->ps | INTERRUPT_MASK;
                this->pc += 0x01;
                break;
            // STA - Store Accumulator in Memory
            case 0x85:
                // Zero-Page
                writeZeroPage(this->accumulator, 0x00);
                this->pc += 0x02;
                break;
            case 0x95:
                // Zero-Page, X Offset
                writeZeroPage(this->accumulator, this->regX);
                this->pc += 0x02;
                break;
            case 0x8d:
                // Absolute
                writeAbsolute(this->accumulator, 0x00);
                this->pc += 0x03;
                break;
            case 0x9d:
                // Absolute, X Offset
                writeAbsolute(this->accumulator, this->regX);
                this->pc += 0x03;
                break;
            case 0x99:
                // Absolute, Y Offset
                writeAbsolute(this->accumulator, this->regY);
                this->pc += 0x03;
                break;
            case 0x81:
                // Indirect, X Offset
            case 0x91:
                // Indirect, Y Offset
                std::cout << "STA\n";
                break;
            // STX - Store Index X in Memory
            case 0x86:
                // Zero-Page
                writeZeroPage(this->regX, 0x00);
                this->pc += 0x02;
                break;
            case 0x96:
                // Zero-Page + Y Offset
                writeZeroPage(this->regX, this->regY);
                this->pc += 0x02;
                break;
            case 0x8e:
                // Absolute
                writeAbsolute(this->regX, 0x00);
                this->pc += 0x03;
                break;
            // STY - Store Index Y in Memory
            case 0x84:
                // Zero-Page
                writeZeroPage(this->regY, 0x00);
                this->pc += 0x02;
                break;
            case 0x94:
                // Zero-Page + X Offset
                writeZeroPage(this->regY, this->regX);
                this->pc += 0x02;
                break;
            case 0x8c:
                // Absolute
                writeAbsolute(this->accumulator, 0x00);
                this->pc += 0x03;
                break;
            // TAX - Transfer Accumulator to Index X
            case 0xaa:
                this->regX = this->accumulator;
                setNegative(this->regX);
                setZero(this->regX);
                this->pc += 0x01;
                break;
            // TAY - Transfer Accumulator to Index Y
            case 0xa8:
                this->regY = this->accumulator;
                setNegative(this->regY);
                setZero(this->regY);
                this->pc += 0x01;
                break;
            // TSX - Transfer Stack Pointer to Index X
            case 0xba:
                this->regX = this->sp;
                setNegative(this->regX);
                setZero(this->regX);
                this->pc += 0x01;
                break;
            // TXA - Transfer Index X to Accumulator
            case 0x8a:
                this->accumulator = this->regX;
                setNegative(this->accumulator);
                setZero(this->accumulator);
                this->pc += 0x01;
                break;
            // TXS - Transfer Index X to Stack Pointer
            case 0x9a:
                this->sp = this->regX;
                this->pc += 0x01;
                break;
            // TYA - Transfer Index Y to Accumulator
            case 0x98:
                this->accumulator = this->regY;
                setNegative(this->accumulator);
                setZero(this->accumulator);
                this->pc += 0x01;
                break;
            default:
                std::cout << "Opcode Not Implemented. \n";
                break;
        }
    }
};
