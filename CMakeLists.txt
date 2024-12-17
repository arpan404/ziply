cmake_minimum_required(VERSION 3.10)
project(ziply)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find LZMA package
find_package(LibLZMA REQUIRED)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Add executable
add_executable(ziply src/main.cpp src/parser.cpp src/ziply.cpp src/ende.cpp src/generator.cpp src/error.cpp src/file.cpp src/threadPool.cpp src/stb_image_write.cpp)

# Include directories
target_include_directories(ziply PRIVATE 
    include
    ${LIBLZMA_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

# Link against LZMA
target_link_libraries(ziply PRIVATE ${LIBLZMA_LIBRARIES})

# Link against OpenSSL
target_link_libraries(ziply PRIVATE OpenSSL::Crypto ${OpenCV_LIBS})

