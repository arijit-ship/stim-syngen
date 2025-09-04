cmake_minimum_required(VERSION 3.13)
project(stim_syngen)

set(CMAKE_CXX_STANDARD 20)

# --- Local STIM ---
include_directories(${CMAKE_SOURCE_DIR}/external/stim/out_install/include)
link_directories(${CMAKE_SOURCE_DIR}/external/stim/out_install/lib)

# --- Add yaml-cpp locally ---
# Make sure you cloned yaml-cpp into external/yaml-cpp
add_subdirectory(external/yaml-cpp)

# --- Main programs ---
add_executable(syngen src/syngen.cpp)
target_link_libraries(syngen PRIVATE stim yaml-cpp)

add_executable(main_prog src/main.cpp)
target_link_libraries(main_prog PRIVATE stim yaml-cpp)

# --- Measurement program ---
add_executable(measure src/measure.cpp)
target_link_libraries(measure PRIVATE stim yaml-cpp)

# --- Measurement test ---
add_executable(mestest src/mestest.cpp)
target_link_libraries(mestest PRIVATE stim yaml-cpp)
