cmake_minimum_required(VERSION 3.10.0)
project (quantik) 

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include all source files in the current directory
file(GLOB SOURCES "*.cpp")

# Include all header files in the current directory
file(GLOB HEADERS "*.hpp")

# Add executable target
add_executable(quantik ${SOURCES} ${HEADERS})
