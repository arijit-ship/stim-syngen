#include "stim.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

int main() {
    using namespace std;
    using namespace stim;
    using namespace stim_draw_internal;

    std::cout << "HELLO MESTEST" << std::endl;

    // --- 0. Parameters ---
    size_t shots = 1;         // number of samples
    uint64_t seed = 42;       // deterministic RNG seed

    // --- 1. Load circuit from file ---
    std::ifstream circ_file("test_circ.txt");
    if (!circ_file.is_open()) {
        // Try parent folder
        circ_file.open("../test_circ.txt");
        if (!circ_file.is_open()) {
            std::cerr << "Failed to open test_circ.txt in current or parent directory!" << std::endl;
            return 1;
        }
    }

    std::stringstream buffer;
    buffer << circ_file.rdbuf();
    std::string circuit_str = buffer.str();
    circ_file.close();

    Circuit circuit(circuit_str);

    std::cout << "Circuit has " 
              << circuit.count_qubits() << " qubits and "
              << circuit.count_measurements() << " measurements.\n";

    // --- 2. Draw SVG ---
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

    // --- 3. Sample measurements ---
    std::mt19937_64 rng(seed);  // deterministic RNG
    simd_bits<MAX_BITWORD_WIDTH> ref_sample = TableauSimulator<MAX_BITWORD_WIDTH>::reference_sample_circuit(circuit);
    auto samples = sample_batch_measurements(circuit, ref_sample, shots, rng, false); // transposed=false

    // --- 4. Print all bits with labels and newlines ---
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
