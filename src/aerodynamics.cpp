#include "aerodynamics.hpp"
#include <algorithm>
#include <cmath>

AeroForces aero_forces(float q_bar, float Aref_m2, float CD, float CY,
                       float CL) {
  AeroForces f = {};

  f.drag_N = CD * q_bar * Aref_m2;
  f.side_N = -CY * q_bar * Aref_m2;
  f.lift_N = CL * q_bar * Aref_m2;

  return f;
};

BodyForces wind_to_body(AeroForces f, float alpha_rad, float beta_rad) {
  float s_alpha = std::sin(alpha_rad);
  float c_alpha = std::cos(alpha_rad);
  float s_beta = std::sin(beta_rad);
  float c_beta = std::cos(beta_rad);

  BodyForces bodyF = {};

  bodyF.Fx_N = -(c_alpha * c_beta * f.drag_N - c_alpha * s_beta * f.side_N -
                 s_alpha * f.lift_N);
  bodyF.Fy_N = -(s_beta * f.drag_N + c_beta * f.side_N);
  bodyF.Fz_N = -(s_alpha * c_beta * f.drag_N - s_alpha * s_beta * f.side_N +
                 c_alpha * f.lift_N);

  return bodyF;
};