#include "stim.h"
#include <iostream>
#include <fstream>
#include <random>

int main() {
    using namespace stim;
    using namespace stim_draw_internal;

    // --- 0. Parameters ---
    size_t distance = 3;      // code distance
    size_t rounds = 1;        // number of rounds
    size_t shots = 1;         // number of samples
    uint64_t seed = 42;       // deterministic RNG seed

    // --- 1. Circuit generation parameters ---
    CircuitGenParameters params(rounds, distance, "rotated_memory_z");
    params.after_clifford_depolarization = 0;
    params.before_round_data_depolarization = 0;
    params.before_measure_flip_probability = 0;
    params.after_reset_flip_probability = 0;
    params.validate_params();

    // --- 2. Generate the surface code circuit ---
    GeneratedCircuit gen_circ = generate_surface_code_circuit(params);
    const Circuit &circuit = gen_circ.circuit;

    std::cout << "Circuit has " 
              << circuit.count_qubits() << " qubits and "
              << circuit.count_measurements() << " measurements.\n";

    // --- 3. Draw SVG ---
    std::ofstream svg_file("MEASURE_circuit.svg");
    DiagramTimelineSvgDrawer::make_diagram_write_to(
        circuit,
        svg_file,
        0,
        circuit.count_ticks(),
        DiagramTimelineSvgDrawerMode::SVG_MODE_TIMELINE,
        {},
        0
    );
    svg_file.close();

    // --- 4. Sample measurements ---
    std::mt19937_64 rng(seed);  // deterministic RNG
    simd_bits<MAX_BITWORD_WIDTH> ref_sample = TableauSimulator<MAX_BITWORD_WIDTH>::reference_sample_circuit(circuit);
    auto samples = sample_batch_measurements(circuit, ref_sample, shots, rng, false); // transposed=false

    // --- 5. Print all bits with labels and newlines ---
    for (size_t shot = 0; shot < shots; shot++) {
        std::cout << "Shot " << shot << ":\n";

        // Reference bits
        std::cout << "  Reference: ";
        for (size_t m = 0; m < circuit.count_measurements(); m++) {
            std::cout << ref_sample[m];
        }
        std::cout << std::endl;

        // Measured bits
        std::cout << "  Measured : ";
        for (size_t m = 0; m < circuit.count_measurements(); m++) {
            std::cout << samples[m][shot];
        }
        std::cout << std::endl;

        // Flip bits (ref ^ measured)
        std::cout << "  Is flipped     : ";
        for (size_t m = 0; m < circuit.count_measurements(); m++) {
            std::cout << (ref_sample[m] ^ samples[m][shot]);
        }
        std::cout << std::endl;
    }

    return 0;
}
