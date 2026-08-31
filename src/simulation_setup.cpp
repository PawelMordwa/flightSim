#include "simulation_setup.hpp"
#include <cmath>

Simulation::Simulation() {
  t0 = 0;
  tf = 50;
  ts = 0.005;

  x0 = Simulation::simulation_set_initial_conditions();

  int nt_s = static_cast<int>(std::floor((tf - t0) / ts)) + 1;

  t.resize(nt_s);

  for (int i = 0; i < nt_s; i++) {
    t.at(i) = t0 + ts * i;
  }

  x.assign(state_vector_size, std::vector<float>(nt_s, 0.0f));

  for (int i = 0; i < state_vector_size; i++) {
    x[i][0] = x0[i];
  }
}

std::vector<float> Simulation::simulation_set_initial_conditions(void) {
  std::vector<float> x(state_vector_size);
  x.at(0) = 0.001;
  x.at(1) = 0;
  x.at(2) = 0;
  x.at(3) = 0;
  x.at(4) = 0;
  x.at(5) = 0;
  x.at(6) = 0 * M_PI / 180;
  x.at(7) = -90 * M_PI / 180;
  x.at(8) = 0;
  x.at(9) = 0;
  x.at(10) = 0;
  x.at(11) = -5000;

  return x;
}