cmake_minimum_required(VERSION 3.10.0)
project (quantik) 

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# # Profiling
# set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

# Include all source files in the current directory
file(GLOB SOURCES "*.cpp")

# Include all header files in the current directory
file(GLOB HEADERS "*.hpp")

# Add executable target
add_executable(quantik ${SOURCES} ${HEADERS})
