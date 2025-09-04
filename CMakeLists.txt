cmake_minimum_required(VERSION 3.13)
project(stim_syngen)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Local STIM ---
include_directories(${CMAKE_SOURCE_DIR}/external/stim/out_install/include)
link_directories(${CMAKE_SOURCE_DIR}/external/stim/out_install/lib)

# --- Add yaml-cpp locally ---
# Make sure you cloned yaml-cpp into external/yaml-cpp
if(EXISTS "${CMAKE_SOURCE_DIR}/external/yaml-cpp/CMakeLists.txt")
    add_subdirectory(external/yaml-cpp)
else()
    message(FATAL_ERROR "yaml-cpp not found in external/yaml-cpp. Please clone it first.")
endif()

# --- Main executables ---
# add_executable(syngen src/syngen.cpp)
# target_link_libraries(syngen PRIVATE stim yaml-cpp)

# Optional: keep main.cpp for testing if needed
if(EXISTS "${CMAKE_SOURCE_DIR}/src/main.cpp")
    add_executable(main_prog src/main.cpp)
    target_link_libraries(main_prog PRIVATE stim yaml-cpp)
endif()

# --- Measurement programs ---
if(EXISTS "${CMAKE_SOURCE_DIR}/src/measure.cpp")
    add_executable(measure src/measure.cpp)
    target_link_libraries(measure PRIVATE stim yaml-cpp)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/mestest.cpp")
    add_executable(mestest src/mestest.cpp)
    target_link_libraries(mestest PRIVATE stim yaml-cpp)
endif()

add_executable(syngen
    src/syngen.cpp
    src/syngen/SurfaceCodeSimulatorWrapper.cpp  # << add this
)

target_link_libraries(syngen
    yaml-cpp
    stim
)

