cmake_minimum_required(VERSION 3.6)
project(fluid_sim)

# Include SDL libs
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-build-debug)
set(CMAKE_CXX_STANDARD 14)

# Debug
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -v")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

# Fluid executable
file(GLOB FLUID_SOURCES fluid/*.cpp)
add_executable(fluid_sim ${FLUID_SOURCES})

# RTInstability executable
#file(GLOB RTINSTABILITY_SOURCES rtinstability/*.cpp)
#add_executable(rtinstability_sim ${RTINSTABILITY_SOURCES})

# Link to SDL
target_link_libraries(fluid_sim ${SDL2_LIBRARY})