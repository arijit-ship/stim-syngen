#pragma once
#include "stim.h"
#include <string>

class SurfaceCodeSimulatorWrapper {
public:
    SurfaceCodeSimulatorWrapper(
        size_t distance,
        size_t rounds,
        size_t shots,
        uint64_t seed,
        const std::string &code_type,
        double after_clifford_depolarization,
        double before_round_data_depolarization,
        double before_measure_flip_probability,
        double after_reset_flip_probability
    );

    void generate_circuit();
    void draw_circuit_svg(const std::string &filename);
    void sample_measurements();

private:
    size_t distance;
    size_t rounds;
    size_t shots;
    uint64_t seed;
    std::string code_type;

    double after_clifford_depolarization;
    double before_round_data_depolarization;
    double before_measure_flip_probability;
    double after_reset_flip_probability;

    stim::GeneratedCircuit gen_circ;
};
