cmake_minimum_required(VERSION 3.10)
project(Ziply)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add executable
add_executable(Ziply src/main.cpp src/Ziply.cpp src/Error.cpp src/File.cpp)

# Include directories
target_include_directories(Ziply PRIVATE 
    include
    "/opt/homebrew/Cellar/imagemagick/7.1.1-41/include/ImageMagick-7"
)

# Define necessary macros
target_compile_definitions(Ziply PRIVATE 
    MAGICKCORE_QUANTUM_DEPTH=16
    MAGICKCORE_HDRI_ENABLE=1
)

# Link libraries
target_link_libraries(Ziply PRIVATE 
    "/opt/homebrew/Cellar/imagemagick/7.1.1-41/lib/libMagick++-7.Q16HDRI.dylib"
    "/opt/homebrew/Cellar/imagemagick/7.1.1-41/lib/libMagickWand-7.Q16HDRI.dylib"
    "/opt/homebrew/Cellar/imagemagick/7.1.1-41/lib/libMagickCore-7.Q16HDRI.dylib"
)
