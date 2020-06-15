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
        this->memory = memory;
        pc = 0x0000;
        sp = 0xff;
        ps = 0x00;
    }

    void emulateOp() {
        uint8_t opCode = this->memory.readIndex(pc);
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
                std:: cout << "BCC\n";
                break;
            // BCS - Branch on Carry Set
            case 0xb0:
                std::cout << "BCS\n";
                break;
            // BEQ - Branch on Result Zero
            case 0xf0:
                std::cout << "BEQ\n";
                break;
            // BIT - Test Bits in Memory with Accumulator
            case 0x24:
            case 0x2c:
                std::cout << "BIT\n";
                break;
            // BMI - Branch on Result Minus
            case 0x30:
                std::cout << "BMI\n";
                break;
            // BNE - Branch on Result not Zero
            case 0xd0:
                std::cout << "BNE\n";
                break;
            // BPL - Branch on Result Plus
            case 0x10:
                std::cout << "BPL\n";
                break;
            // BRK - Force Break
            case 0x00:
                std::cout << "BRK\n";
                break;
            // BVC - Branch on Overflow Clear
            case 0x50:
                std::cout << "BVC\n";
                break;
            // BVS - Branch on Overflow Set
            case 0x70:
                std::cout << "BVC\n";
                break;
            // CLC - Clear Carry Flag
            case 0x18:
                this->ps = this->ps | (~CARRY_MASK);
                this->pc += 1;
            // CLD - Clear Decimal Mode
            case 0xd8:
                this->ps = this->ps | (~DECIMAL_MASK);
                this->pc += 1;
            // CLI - Clear Interrupt Disable Bit
            case 0x58:
                this->ps = this->ps | (~INTERRUPT_MASK);;
                this->pc += 1;
            // CLV - Clear Overflow Flag
            case 0xb8:
                this->ps = this->ps | (~OVERFLOW_MASK);
                this->pc += 1;
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
            // DEX - Decrement Index X by One
            case 0xca:
                std::cout << "DEX\n";
                this->pc += 1;
            // DEY - Decrement Index Y by One
            case 0x88:
                std::cout << "DEY\n";
                this->pc += 1;
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
                this->pc += 1;
                break;
            // INY - Increment Index Y by One
            case 0xc8:
                std::cout << "INY\n";
                this->pc += 1;
                break;
            // JMP - Jumpt to New Location
            case 0x4c:
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
            case 0xa6:
            case 0xb6:
            case 0xae:
            case 0xbe:
                std::cout << "LDX\n";
            // LDY - Load Index Y with Memory
            case 0xa0:
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
                this->pc += 1;
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
                std::cout << "PHA\n";
                this->pc += 1;
                break;
            // PHP - Push Processor Status on Stack
            case 0x08:
                std::cout << "PHP\n";
                this->pc += 1;
                break;
            // PLA - Pull Accumulator from Stack
            case 0x68:
                std::cout << "PLA\n";
                this->pc += 1;
                break;
            // PLP - Pull Processor Status from Stack
            case 0x28:
                std::cout << "PLP\n";
                this->pc += 1;
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
                this->pc += 1;
                break;
            // RTS - Return from Subroutine
            case 0x60:
                std::cout << "RTS\n";
                this->pc += 1;
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
                this->pc += 1;
                break;
            // SED - Set Decimal Flag
            case 0xf8:
                this->ps = this->ps | DECIMAL_MASK;
                this->pc += 1;
                break;
            // SEI - Set Interrupt Disable Status
            case 0x78:
                this->ps = this->ps | INTERRUPT_MASK;
                this->pc += 1;
                break;
            // STA - Store Accumulator in Memory
            case 0x85:
            case 0x95:
            case 0x8d:
            case 0x9d:
            case 0x99:
            case 0x81:
            case 0x91:
                std::cout << "STA\n";
                break;
            // STX - Store Index X in Memory
            case 0x86:
            case 0x96:
            case 0x8e:
                std::cout << "STX\n";
                break;
            // STY - Store Index Y in Memory
            case 0x84:
            case 0x94:
            case 0x8c:
                std::cout << "STY\n";
                break;
            // TAX - Transfer Accumulator to Index X
            case 0xaa:
                std::cout << "TAX\n";
                this->pc += 1;
                break;
            // TAY - Transfer Accumulator to Index Y
            case 0xa8:
                std::cout << "TAY\n";
                this->pc += 1;
                break;
            // TSX - Transfer Stack Pointer to Index X
            case 0xba:
                std::cout << "TSX\n";
                this->pc += 1;
                break;
            // TXA - Transfer Index X to Accumulator
            case 0x8a:
                std::cout << "TXA\n";
                this->pc += 1;
                break;
            // TXS - Transfer Index X to Stack Pointer
            case 0x9a:
                std::cout << "TXS\n";
                this->pc += 1;
                break;
            // TYA - Transfer Index Y to Accumulator
            case 0x98:
                std::cout << "TYA\n";
                this->pc += 1;
                break;
            default:
                std::cout << "Opcode Not Implemented. \n";
                break;
        }
    }
};
