cmake_minimum_required(VERSION 3.10.0)
project (quantik)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Data Generation
file(GLOB P1_SOURCES "./data_generation/*.cpp")
file(GLOB P1_HEADERS "./data_generation/*.hpp")
add_executable(data_generation ${P1_SOURCES} ${P1_HEADERS})
