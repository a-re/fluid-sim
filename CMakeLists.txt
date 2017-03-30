cmake_minimum_required(VERSION 3.6)
project(fluid_sim)

# Include SDL libs
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES fluid/fluid.cpp)
add_executable(fluid_sim ${SOURCE_FILES})

# Link to SDL
target_link_libraries(fluid_sim libSDL2.a)