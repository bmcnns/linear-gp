//
// Created by bryce on 12/10/24.
//

#ifndef PROGRAM_H
#define PROGRAM_H
#include <array>
#include <cstdint>
#include <vector>
#include "Parameters.h"

class Program {
private:
    std::array<double, Parameters::NUM_REGISTERS> registers;

    int getRandomNumber(int min, int max);


public:
    std::vector<uint16_t> instructions;

    void addRandomInstruction();

    void addInstruction(uint16_t instruction);

    Program();

    void execute(const std::vector<float> &features);

    void displayInstructions() const;

    void displayRegisters() const;

    void displayCode() const;

    void mutate();
};

#endif //PROGRAM_H
