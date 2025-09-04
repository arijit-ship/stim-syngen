/*
The entrypoint of stim-syngen.
*/

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>

int main() {
    try {
        // Load YAML config
        YAML::Node config = YAML::LoadFile("config.yml");

        // --- Task ---
        std::string task = config["task"].as<std::string>();
        std::cout << "Task: " << task << std::endl;

        // --- Parameters ---
        YAML::Node params = config["parameters"];
        int distance = params["distance"].as<int>();
        int rounds = params["rounds"].as<int>();

        std::cout << "Distance: " << distance << ", Rounds: " << rounds << std::endl;

        // Errors
        YAML::Node errors = params["errors"];
        double after_cliff = errors["after_clifford_depolarization"].as<double>();
        double before_round = errors["before_round_data_depolarization"].as<double>();
        double before_measure = errors["before_measure_flip_probability"].as<double>();
        double after_reset = errors["after_reset_flip_probability"].as<double>();

        std::cout << "Errors:\n"
                  << "  after_clifford: " << after_cliff << "\n"
                  << "  before_round: " << before_round << "\n"
                  << "  before_measure: " << before_measure << "\n"
                  << "  after_reset: " << after_reset << std::endl;

        // Sampling
        YAML::Node sampling = params["sampling"];
        int shots = sampling["shots"].as<int>();
        YAML::Node seed_node = sampling["seed"];
        if (seed_node.IsNull()) {
            std::cout << "Seed: Null" << std::endl;
        } else {
            uint64_t seed = seed_node.as<uint64_t>();
            std::cout << "Seed: " << seed << std::endl;
        }
        bool console_log = sampling["console_log"].as<bool>();
        std::cout << "Sampling console_log: " << console_log << std::endl;

        // Exports
        YAML::Node exports = config["exports"];
        std::string figure_file = exports["figure"]["file"].as<std::string>();
        std::string circ_file = exports["circuit"]["file"].as<std::string>();
        std::string output_file = exports["output"]["file"].as<std::string>();
        bool prettify = exports["output"]["prettify"].as<bool>();

        std::cout << "Figure file: " << figure_file << "\n"
                  << "Circuit file: " << circ_file << "\n"
                  << "Output file: " << output_file
                  << ", Prettify: " << prettify << std::endl;

    } catch (const YAML::Exception &e) {
        std::cerr << "YAML Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
