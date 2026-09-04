#include "simulation_setup.hpp"
#include <cmath>

Simulation::Simulation(std::vector<float> initialState) {
  t0 = 0;
  tf = 50;
  ts = 0.005;

  x0 = initialState;

  nt_s = static_cast<int>(std::floor((tf - t0) / ts)) + 1;

  t.resize(nt_s);

  for (int i = 0; i < nt_s; i++) {
    t.at(i) = t0 + ts * i;
  }

  x.assign(state_vector_size, std::vector<float>(nt_s, 0.0f));

  for (int i = 0; i < state_vector_size; i++) {
    x[i][0] = x0[i];
  }
}