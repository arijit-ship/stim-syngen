#include "SurfaceCodeSimulatorWrapper.h"
#include "stim.h"
#include <iostream>
#include <fstream>
#include <random>

using namespace stim;
using namespace stim_draw_internal;

SurfaceCodeSimulatorWrapper::SurfaceCodeSimulatorWrapper(
    size_t distance,
    size_t rounds,
    size_t shots,
    uint64_t seed,
    const std::string &code_type,
    double after_clifford_depolarization,
    double before_round_data_depolarization,
    double before_measure_flip_probability,
    double after_reset_flip_probability
) : distance(distance),
    rounds(rounds),
    shots(shots),
    seed(seed),
    code_type(code_type),
    after_clifford_depolarization(after_clifford_depolarization),
    before_round_data_depolarization(before_round_data_depolarization),
    before_measure_flip_probability(before_measure_flip_probability),
    after_reset_flip_probability(after_reset_flip_probability) {}

void SurfaceCodeSimulatorWrapper::generate_circuit() {
    CircuitGenParameters params(rounds, distance, code_type);
    params.after_clifford_depolarization = after_clifford_depolarization;
    params.before_round_data_depolarization = before_round_data_depolarization;
    params.before_measure_flip_probability = before_measure_flip_probability;
    params.after_reset_flip_probability = after_reset_flip_probability;
    params.validate_params();

    gen_circ = generate_surface_code_circuit(params);

    const Circuit &circuit = gen_circ.circuit;
    std::cout << "Circuit has "
              << circuit.count_qubits() << " qubits and "
              << circuit.count_measurements() << " measurements.\n";
}

void SurfaceCodeSimulatorWrapper::draw_circuit_svg(const std::string &filename, bool transparent) {
    // 1. Generate the SVG via STIM
    std::ofstream svg_file(filename);
    DiagramTimelineSvgDrawer::make_diagram_write_to(
        gen_circ.circuit,
        svg_file,
        0,
        gen_circ.circuit.count_ticks(),
        DiagramTimelineSvgDrawerMode::SVG_MODE_TIMELINE,
        {},
        0
    );
    svg_file.close();

    // 2. If transparent == false, inject white background
    if (!transparent) {
        std::ifstream in(filename);
        std::string svg((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();

        size_t insert_pos = svg.find('>'); // end of <svg ...> tag
        if (insert_pos != std::string::npos) {
            svg.insert(insert_pos + 1, "<rect width=\"100%\" height=\"100%\" fill=\"white\" />\n");
        }

        std::ofstream out(filename);
        out << svg;
        out.close();
    }
}


void SurfaceCodeSimulatorWrapper::sample_measurements() {
    const Circuit &circuit = gen_circ.circuit;

    std::mt19937_64 rng(seed);
    simd_bits<MAX_BITWORD_WIDTH> ref_sample = TableauSimulator<MAX_BITWORD_WIDTH>::reference_sample_circuit(circuit);
    auto samples = sample_batch_measurements(circuit, ref_sample, shots, rng, false);

    std::cout << "SAMPLES: " << samples << std::endl;
    std::cout << "REFERENCE: " << ref_sample << std::endl;

    for (size_t shot = 0; shot < shots; shot++) {
        std::cout << "Shot " << shot << ":\n";

        std::cout << "  Reference: ";
        for (size_t m = 0; m < circuit.count_measurements(); m++) {
            std::cout << ref_sample[m];
        }
        std::cout << std::endl;

        std::cout << "  Measured : ";
        for (size_t m = 0; m < circuit.count_measurements(); m++) {
            std::cout << samples[m][shot];
        }
        std::cout << std::endl;

        std::cout << "  Is flipped: ";
        for (size_t m = 0; m < circuit.count_measurements(); m++) {
            std::cout << (ref_sample[m] ^ samples[m][shot]);
        }
        std::cout << std::endl;
    }
}

void SurfaceCodeSimulatorWrapper::export_circuit_txt(const std::string &filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Failed to open file for writing circuit: " << filename << std::endl;
        return;
    }

    out << gen_circ.circuit << std::endl;
    out.close();

    std::cout << "Circuit exported to: " << filename << std::endl;
}

