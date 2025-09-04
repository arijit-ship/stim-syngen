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
        cout << "YAML config has been loaded successfully!\n" << endl;
    } catch (const std::exception &e) {
        cerr << "Failed to load config file: " << e.what() << endl;
        return 1;
    }

    // Task string
    string task = config["task"].as<string>();

    // --- Parameters ---
    YAML::Node params = config["parameters"];
    size_t distance = params["distance"].as<size_t>();
    size_t rounds   = params["rounds"].as<size_t>();

    // Errors
    YAML::Node errors = params["errors"];
    double after_clifford_depolarization    = errors["after_clifford_depolarization"].as<double>();
    double before_round_data_depolarization = errors["before_round_data_depolarization"].as<double>();
    double before_measure_flip_probability  = errors["before_measure_flip_probability"].as<double>();
    double after_reset_flip_probability     = errors["after_reset_flip_probability"].as<double>();

    // Sampling
    YAML::Node sampling = params["sampling"];
    uint64_t seed = sampling["seed"].IsNull() ? 42 : sampling["seed"].as<uint64_t>();
    size_t shots  = sampling["shots"].as<size_t>();
    bool console_log = sampling["console_log"].as<bool>();

    // --- Simulator ---
    SurfaceCodeSimulatorWrapper sim(
        distance,
        rounds,
        shots,
        seed,
        task, // task string passed as code_type
        after_clifford_depolarization,
        before_round_data_depolarization,
        before_measure_flip_probability,
        after_reset_flip_probability
    );

    sim.generate_circuit();

    // Exports
    YAML::Node exports = config["exports"];
    if (exports["figure"] && exports["figure"]["exporting"].as<bool>()) {
        std::string fig_file = exports["figure"]["file"].as<string>();
        bool trans_bg = false; // default if key missing
        if (exports["figure"]["trans_bg"]) {
            trans_bg = exports["figure"]["trans_bg"].as<bool>();
        }
        sim.draw_circuit_svg(fig_file, trans_bg);
    }

    sim.sample_measurements();

    if (exports["circuit"] && exports["circuit"]["exporting"].as<bool>()) {
    std::string circ_file = exports["circuit"]["file"].as<std::string>();
    sim.export_circuit_txt(circ_file);
}


    return 0;
}
