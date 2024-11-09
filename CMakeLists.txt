cmake_minimum_required(VERSION 3.28)
project(linear_gp)

# Ensure C++20 standard is used
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find PyBind11, installed via pip or system
find_package(pybind11 REQUIRED)

# Add the shared library for the Python module
pybind11_add_module(linear_gp python_bindings.cpp Program.cpp Mutator.cpp)

# Install the Python module to the python/linear_gp directory
install(TARGETS linear_gp
        LIBRARY DESTINATION gp-is-good-for-fqe/linear_gp)