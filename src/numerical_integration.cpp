#include "numerical_integration.hpp"
#include "model_equations.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

void forward_euler(Simulation &sim) {
  int nt_s = static_cast<int>(std::floor((sim.tf - sim.t0) / sim.ts)) + 1;

  std::vector<float> x_prev(sim.x0);
  std::vector<float> dx(state_vector_size);

  for (size_t k = 1; k < nt_s; k++) {
    std::transform(sim.x.begin(), sim.x.end(), x_prev.begin(),
                   [k](const std::vector<float> &row) { return row[k - 1]; });
    dx = model_equations(x_prev);

    for (size_t i = 0; i < state_vector_size; i++) {
      sim.x[i][k] = sim.x[i][k - 1] + sim.ts * dx[i];
    }
  }
}