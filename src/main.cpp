#include "model_equations.hpp"
#include "numerical_integration.hpp"
#include "simulation_setup.hpp"
#include "vehicle_model.hpp"
#include <cmath>
#include <fstream>
#include <vector>

int main(void) {
  set_brick_model();

  std::vector<float> initialState = {0.001,
                                     0,
                                     0,
                                     10 * M_PI / 180,
                                     20 * M_PI / 180,
                                     30 * M_PI / 180,
                                     0 * M_PI / 180,
                                     0 * M_PI / 180,
                                     0 * M_PI / 180,
                                     0,
                                     0,
                                     -5000};

  Simulation sim(initialState);

  forward_euler(sim);

  std::ofstream file("sim_output.csv");
  file << "t_s,u,v,w,p,q,r,phi,theta,psi,px,py,pz\n";

  for (auto k = 0; k < sim.t.size(); k++) {
    file << sim.t[k];
    for (auto i = 0; i < state_vector_size; i++) {
      file << "," << sim.x[i][k];
    }
    file << "\n";
  }

  return 0;
}