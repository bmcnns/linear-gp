//
// Created by bryce on 12/10/24.
//

#include "Mutator.h"

#include <bitset>
#include <iostream>

std::mt19937 Mutator::gen(std::random_device{}());
std::uniform_real_distribution<> Mutator::rng(0.0, 1.0);

void Mutator::mutateProgram(Program &program) {
    if (rng(gen) < Parameters::ADD_INSTRUCTION_PROBABILITY) {
        if (program.instructions.size() < Parameters::MAX_PROGRAM_LENGTH) {
            program.addRandomInstruction();
        }
    }
    if (rng(gen) < Parameters::DELETE_INSTRUCTION_PROBABILITY) {
        if (program.instructions.size() > 1) {
            std::uniform_int_distribution<> distrib(0, program.instructions.size() - 1);
            int index = distrib(gen);
            program.instructions.erase(program.instructions.begin() + index);
        }
    }
    if (rng(gen) < Parameters::SWAP_INSTRUCTION_PROBABILITY) {
        if (program.instructions.size() > 1) {
            std::uniform_int_distribution<> distrib(0, program.instructions.size() - 1);
            int idx1 = distrib(gen);
            int idx2 = distrib(gen);

            while (idx2 == idx1) {
                idx2 = distrib(gen);
            }

            std::swap(program.instructions[idx1], program.instructions[idx2]);
        }
    }

    for (uint16_t& instruction : program.instructions) {
        if (rng(gen) < Parameters::MUTATE_INSTRUCTION_PROBABILITY) {
            mutateInstruction(instruction);
        }
    }
}

void Mutator::mutateInstruction(uint16_t &instruction) {
    const int INSTRUCTION_NUM_PARTS = 4;
    std::uniform_int_distribution<> distrib(0, INSTRUCTION_NUM_PARTS - 1);
    int index = distrib(gen);

    // Mutate the mode bit
    if (index == 0) {
        // Flips the 14th bit (mode bit)
        instruction ^= (1 << 14);

        // If the mode bit is 0, then we're using internal registers.
        // Make sure the source register is within the bounds.
        if (((instruction >> 14) & 1) == 0) {
            int src = (instruction) & 0xFF;
            src = src % Parameters::NUM_REGISTERS;
            instruction &= ~(0xFF); // Clear the current source register
            instruction |= src; // Set the source register to the new clipped value
        }
    }
    // Mutate the op code
    else if (index == 1) {
        int opCode = (instruction >> 8 & 0b111);

        std::uniform_int_distribution<> opDistrib(0, Parameters::NUM_OP_CODES - 1);
        int newOpCode = opDistrib(gen);

        if (newOpCode == opCode) {
            newOpCode = opDistrib(gen);
        }

        // Clear the current op code bits
        instruction &= ~(0b111 << 8);

        // Set the new op code
        instruction |= (newOpCode << 8);
    }
    // Mutate the source register
    else if (index == 2) {
        int src = (instruction & 0xFF);

        int newSrc;
        // We're addressing internal registers (mode bit 0)
        if (((instruction >> 14) & 1) == 0) {
            std::uniform_int_distribution<> registerDistrib(0, Parameters::NUM_REGISTERS - 1);
            newSrc = registerDistrib(gen);

            if (newSrc == src) {
                newSrc == registerDistrib(gen);
            }
        }
        // We're addressing features (mode bit 1)
        else {
            std::uniform_int_distribution<> featureDistrib(0, Parameters::NUM_FEATURES - 1);
            newSrc = featureDistrib(gen);

            while (newSrc == src) {
                newSrc = featureDistrib(gen);
            }
        }

        instruction &= ~(0xFF);
        instruction |= newSrc;
    }
    // Mutate the destination register
    else if (index == 3) {
        int dest = (instruction >> 11 & 0b111);

        std::uniform_int_distribution<> registerDistrib(0, Parameters::NUM_REGISTERS - 1);
        int newDest = registerDistrib(gen);

        while (newDest == dest) {
            newDest = registerDistrib(gen);
        }

        instruction &= ~(0b111 << 11);
        instruction |= (newDest << 11);
    }
    else
    {
        std::cerr << "Unknown mutation" << std::endl;
    }
}

std::pair<Program, Program> Mutator::crossover(const Program &program1, const Program &program2) {
    std::uniform_int_distribution<> dist1(0, program1.instructions.size() - 1);
    std::uniform_int_distribution<> dist2(0, program2.instructions.size() - 1);

    int crossoverPoint1 = dist1(gen);
    int crossoverPoint2 = dist2(gen);

    Program child1, child2;
    child1.instructions.clear();
    child2.instructions.clear();

    for (int i = 0; i < crossoverPoint1; ++i) {
        child1.addInstruction(program1.instructions[i]);
    }

    for (int i = crossoverPoint2; i < program2.instructions.size(); i++) {
        child1.addInstruction(program2.instructions[i]);
    }

    for (int i = 0; i < crossoverPoint2; ++i) {
        child2.addInstruction(program2.instructions[i]);
    }
    for (int i = crossoverPoint1; i < program1.instructions.size(); i++) {
        child2.addInstruction(program1.instructions[i]);
    }

    return {child1, child2};
}
