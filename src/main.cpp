#include "stim.h"
#include <iostream>
#include <fstream>

int main() {
    using namespace stim;
    using namespace stim_draw_internal;

    // --- 1. Circuit generation parameters ---
    CircuitGenParameters params(5, 3, "rotated_memory_z");
    params.after_clifford_depolarization = 0.001;
    params.before_round_data_depolarization = 0.002;
    params.before_measure_flip_probability = 0.01;
    params.after_reset_flip_probability = 0.01;
    params.validate_params();

    // --- 2. Generate the surface code circuit ---
    GeneratedCircuit gen_circ = generate_surface_code_circuit(params);
    const Circuit &circuit = gen_circ.circuit;

    std::cout << "Circuit has " 
              << circuit.count_qubits() << " qubits and "
              << circuit.count_measurements() << " measurements.\n";

    // --- 3. Draw SVG ---
    std::ofstream svg_file("MAIN.CPP.surface_code_circuit.svg");
    if (!svg_file.is_open()) {
        std::cerr << "Failed to open SVG file for writing!\n";
        return 1;
    }

    DiagramTimelineSvgDrawer::make_diagram_write_to(
        circuit,                       // the circuit
        svg_file,                       // output stream
        0,                              // tick_slice_start
        circuit.count_ticks(),          // number of ticks to draw
        DiagramTimelineSvgDrawerMode::SVG_MODE_TIMELINE, // mode
        {},                             // optional coordinate filter (empty = draw all)
        0                               // num_rows (0 = auto)
    );

    svg_file.close();
    std::cout << "SVG written to MAIN.CPP.surface_code_circuit.svg\n";

    return 0;
}
