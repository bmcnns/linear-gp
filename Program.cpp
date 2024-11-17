//
// Created by bryce on 12/10/24.
//

#include "Program.h"

#include <bitset>
#include <iostream>
#include <random>

int Program::getRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void Program::addRandomInstruction() {
    int modeBit = getRandomNumber(0, 1);
    int src = getRandomNumber(0, Parameters::NUM_REGISTERS - 1);
    int opCode = getRandomNumber(0, Parameters::NUM_OP_CODES - 1);

    int dest;
    if (modeBit == 0)
    {
        dest = getRandomNumber(0, Parameters::NUM_REGISTERS - 1);
    }
    else {
        dest = getRandomNumber(0, Parameters::NUM_FEATURES - 1);
    }

    uint16_t instruction = (modeBit << 14) | (src << 11) | (opCode << 8) | dest;
    instructions.push_back(instruction);
}

void Program::addInstruction(uint16_t instruction) {
    instructions.push_back(instruction);
}

Program::Program() {
    int numInstructions = getRandomNumber(1, Parameters::MAX_PROGRAM_LENGTH);
    for (int i = 0; i < numInstructions; ++i) {
        addRandomInstruction();
    }

    registers.fill(0.0);
}

void Program::execute(const std::vector<float>& features) {
    registers.fill(0.0);

    for (const uint16_t instruction : instructions) {
        // Extract mode bit (bit 14)
        bool modeBit = (instruction >> 14) & 1;
        // Extract source register (bits 13-11)
        int dest = (instruction >> 11) & 0b111;
        // Extract opcode (bits 10-8)
        int opCode = (instruction >> 8) & 0b111;
        // Extract destination register (bits 7-0)
        int src = (instruction & 0xFF);

        double sourceValue;
        if (modeBit == 0) {
            sourceValue = registers[src];
        }
        else {
            sourceValue = features[src];
        }

        switch (opCode) {
            case 0:
                registers[dest] += sourceValue;
                break;
            case 1:
                registers[dest] -= sourceValue;
                break;
            case 2:
                registers[dest] *= 2.0;
                break;
            case 3:
                if (sourceValue != 0) {
                    registers[dest] /= sourceValue;
                }
                else {
                    registers[dest] = 0.0;
                }
                break;
            case 4:
                registers[dest] = std::cos(sourceValue);
                break;
            case 5:
                if (registers[dest] < sourceValue) {
                    registers[dest] = -registers[dest];
                }
                break;
            default:
                std::cerr << "Unknown opcode: " << opCode << std::endl;
                break;
        }
    }
}



void Program::displayInstructions() const {
    std::cout << "Program instructions (in binary):" << std::endl;
    for (const auto& inst : instructions) {
        std::cout << std::bitset<15>(inst) << std::endl;
    }
}

void Program::displayRegisters() const {
    std::cout << "Current state of registers\n";
    for (size_t i = 0; i < Parameters::NUM_REGISTERS; ++i) {
        std::cout << "Register[" << i << "] = " << registers[i] << "\n";
    }
}

void Program::displayCode() const {
    for (const auto& instruction : instructions) {
        bool modeBit = (instruction >> 14) & 1;
        int dest = (instruction >> 11) & 0b111;
        int opCode = (instruction >> 8) & 0b111;
        int src = instruction & 0xFF;

        // Build the plain English description
        std::string srcStr;
        std::string destStr = "register[" + std::to_string(dest) + "]";

        if (modeBit == 0) {
            srcStr = "register[" + std::to_string(src) + "]";
        }
        else {
            srcStr = "feature[" + std::to_string(src) + "]";
        }

        std::string operation;
        switch (opCode) {
            case 0:
                operation = "Add " + srcStr + " to " + destStr;
            break;
            case 1:
                operation = "Subtract " + srcStr + " from " + destStr;
            break;
            case 2:
                operation = "Multiply " + destStr + " by " + srcStr;
            break;
            case 3:
                operation = "Divide " + destStr + " by " + srcStr;
            break;
            case 4:
                operation = "Set " + destStr + " to the cosine of " + srcStr;
            break;
            case 5:
                operation = "Negate " + destStr + " if less than " + srcStr;
            break;
            default:
                operation = "Unknown operation";
            break;
        }

        std::cout << operation << std::endl;
    }
}