#include "model_equations.hpp"
#include "numerical_integration.hpp"
#include "simulation_setup.hpp"
#include "vehicle_model.hpp"
#include <cmath>
#include <vector>

bool dragless_tumbling_brick() {

  set_brick_model();
  std::vector<float> initialState = {0,
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

  // Helper variables
  const float Jxx = eg_model[utils::to_idx(ModelProperties::Jxx_b_kgm2)];
  const float Jyy = eg_model[utils::to_idx(ModelProperties::Jyy_b_kgm2)];
  const float Jzz = eg_model[utils::to_idx(ModelProperties::Jzz_b_kgm2)];

  const float p0 = sim.x[3][0];
  const float q0 = sim.x[4][0];
  const float r0 = sim.x[5][0];

  const float p_f = sim.x[3][sim.nt_s - 1];
  const float q_f = sim.x[4][sim.nt_s - 1];
  const float r_f = sim.x[5][sim.nt_s - 1];

  float T_initial =
      0.5 * (Jxx * pow(p0, 2) + Jyy * pow(q0, 2) + Jzz * pow(r0, 2));
  float T_final =
      0.5 * (Jxx * pow(p_f, 2) + Jyy * pow(q_f, 2) + Jzz * pow(r_f, 2));

  float H_initial =
      pow((Jxx * p0), 2) + pow((Jyy * q0), 2) + pow((Jzz * r0), 2);

  float H_final =
      pow((Jxx * p_f), 2) + pow((Jyy * q_f), 2) + pow((Jzz * r_f), 2);

  float T_diference = abs(T_initial - T_final) / std::max(T_initial, T_final);
  float H_diference = abs(H_initial - H_final) / std::max(H_initial, H_final);

  return (T_diference < 0.01f) && (H_diference < 0.01f);
}

int main() { return dragless_tumbling_brick() ? 0 : 1; }