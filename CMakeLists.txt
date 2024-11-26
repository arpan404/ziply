cmake_minimum_required(VERSION 3.10)
project(Ziply)

# Add executable
add_executable(Ziply src/main.cpp src/ziply.cpp)

# Include directories
target_include_directories(Ziply PRIVATE include)
