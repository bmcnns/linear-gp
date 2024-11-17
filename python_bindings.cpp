//
// Created by bryce on 12/10/24.
//
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Mutator.h"
#include "Program.h"

namespace py = pybind11;

PYBIND11_MODULE(linear_gp, m) {
    py::class_<Program>(m, "Program")
    .def(py::init<>())
    .def("execute", &Program::execute)
    .def("displayInstructions", &Program::displayInstructions)
    .def("displayCode", &Program::displayCode)
    .def("displayRegisters", &Program::displayRegisters);

    py::class_<Mutator>(m, "Mutator")
    .def(py::init<>())
    .def("mutateProgram", &Mutator::mutateProgram)
    .def("crossover", &Mutator::crossover);
}