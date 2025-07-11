cmake_minimum_required(VERSION 3.15)
project(Scanner)

# Set C++ standard (optional)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable target from source files
add_executable(Scanner
    src/main.cpp
    src/ProcessScanner/ProcessScanner.cpp
)