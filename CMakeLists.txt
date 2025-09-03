cmake_minimum_required(VERSION 3.13)
project(stim_syngen)

set(CMAKE_CXX_STANDARD 20)

# Point to local Stim install
include_directories(${CMAKE_SOURCE_DIR}/external/stim/out_install/include)
link_directories(${CMAKE_SOURCE_DIR}/external/stim/out_install/lib)

# Main program
add_executable(stim_syngen src/main.cpp)
target_link_libraries(stim_syngen stim)

# Measurement program
add_executable(measure src/measure.cpp)
target_link_libraries(measure stim)

#Measuremnt test
add_executable(mestest src/mestest.cpp)
target_link_libraries(mestest stim)
