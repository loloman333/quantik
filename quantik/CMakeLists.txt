cmake_minimum_required(VERSION 3.10.0)
project (quantik)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Prototype I
file(GLOB P1_SOURCES "./prototype_one/*.cpp")
file(GLOB P1_HEADERS "./prototype_one/*.hpp")
add_executable(prototype_one ${P1_SOURCES} ${P1_HEADERS})

# Prototype II
file(GLOB P2_SOURCES "./prototype_two/*.cpp")
file(GLOB P2_HEADERS "./prototype_two/*.hpp")
add_executable(prototype_two ${P2_SOURCES} ${P2_HEADERS})
