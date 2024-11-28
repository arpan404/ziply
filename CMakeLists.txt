cmake_minimum_required(VERSION 3.10)
project(Ziply)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add executable
add_executable(Ziply src/main.cpp src/ziply.cpp src/error.cpp src/file.cpp src/threadPool.cpp src/stb_image_write.cpp)

# Include directories
target_include_directories(Ziply PRIVATE 
    include
)

