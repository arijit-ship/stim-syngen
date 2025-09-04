#include "syngen/SurfaceCodeSimulatorWrapper.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    using namespace std;

    string config_file = "config.yml";
    if (argc > 1) config_file = argv[1];

    YAML::Node config;
    try {
        config = YAML::LoadFile(config_file);
    } catch (const std::exception &e) {
        cerr << "Failed to load config file: " << e.what() << endl;
        return 1;
    }

    size_t distance = config["distance"] ? config["distance"].as<size_t>() : 3;
    size_t rounds = config["rounds"] ? config["rounds"].as<size_t>() : 1;
    size_t shots = config["shots"] ? config["shots"].as<size_t>() : 1;
    uint64_t seed = config["seed"] ? config["seed"].as<uint64_t>() : 42;
    string code_type = config["code_type"] ? config["code_type"].as<string>() : "rotated_memory_z";

    SurfaceCodeSimulatorWrapper sim(distance, rounds, shots, seed, code_type);
    sim.generate_circuit();
    sim.draw_circuit_svg("MEASURE_circuit.svg");
    sim.sample_measurements();

    return 0;
}
