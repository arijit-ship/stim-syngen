#pragma once
#include "stim.h"
#include <random>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class SurfaceCodeSimulatorWrapper {
public:
    SurfaceCodeSimulatorWrapper(
        size_t distance,
        size_t rounds,
        size_t shots,
        uint64_t seed,
        const std::string &code_type = "rotated_memory_z"
    );

    void generate_circuit();         // Generate the circuit
    void draw_circuit_svg(const std::string &filename); // Draw SVG
    void sample_measurements();      // Sample measurements and print results

private:
    size_t distance;
    size_t rounds;
    size_t shots;
    uint64_t seed;
    std::string code_type;

    stim::GeneratedCircuit gen_circ;
};
