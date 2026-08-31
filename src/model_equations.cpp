#include "model_equations.hpp"
#include "atmosphere.hpp"
#include "phisical_properties.hpp"
#include "vehicle_model.hpp"
#include <algorithm>
#include <cmath>

std::vector<float> model_equations(const std::vector<float> &x) {

  // Create derivative vector
  std::vector<float> dx(12);

  // Curent state to asignement to individual variables
  float u_b_mps = x.at(0);
  float v_b_mps = x.at(1);
  float w_b_mps = x.at(2);
  float p_b_rps = x.at(3);
  float q_b_rps = x.at(4);
  float r_b_rps = x.at(5);
  float phi_rad = x.at(6);
  float theta_rad = x.at(7);
  float psi_rad = x.at(8);
  float px_n_m = x.at(9);
  float py_n_m = x.at(10);
  float pz_n_m = x.at(11);

  // Euler angles trigonometric functions
  float c_phi = std::cos(phi_rad);
  float c_theta = std::cos(theta_rad);
  float c_psi = std::cos(psi_rad);
  float s_phi = std::sin(phi_rad);
  float s_theta = std::sin(theta_rad);
  float s_psi = std::sin(psi_rad);
  float t_theta = std::tan(theta_rad);

  // Asign phisical properties from model
  float m_kg = eg_model[utils::to_idx(ModelProperties::m_kg)];
  float Jxz_b_kgm2 = eg_model[utils::to_idx(ModelProperties::Jxz_b_kgm2)];
  float Jxx_b_kgm2 = eg_model[utils::to_idx(ModelProperties::Jxx_b_kgm2)];
  float Jyy_b_kgm2 = eg_model[utils::to_idx(ModelProperties::Jyy_b_kgm2)];
  float Jzz_b_kgm2 = eg_model[utils::to_idx(ModelProperties::Jzz_b_kgm2)];

  // Altitude
  float h_m = -pz_n_m;

  // Atmosphere model
  float rho_m = atmosphere::get_density(h_m);

  // Air data
  float airspeed = std::sqrt(std::pow(u_b_mps, 2) + std::pow(v_b_mps, 2) +
                             std::pow(w_b_mps, 2));
  float dynamic_preasure = 0.5 * rho_m * std::pow(airspeed, 2);

  float v_over_VT = 0;

  if (airspeed != 0) {
    v_over_VT = std::max(-1.0f, std::min(1.0f, v_b_mps / airspeed));
  }

  float alpha_rad = std::atan2(w_b_mps, u_b_mps);
  float beta_rad = std::asin(v_over_VT);
  float s_alpha = std::sin(alpha_rad);
  float c_alpha = std::cos(alpha_rad);
  float s_beta = std::sin(beta_rad);
  float c_beta = std::cos(beta_rad);

  // Gravity in body cordinate system
  float gz_n_mps2 = phisical_properies::g;

  float gx_b_mps2 = -std::sin(theta_rad) * gz_n_mps2;
  float gy_b_mps2 = std::sin(phi_rad) * std::cos(theta_rad) * gz_n_mps2;
  float gz_b_mps2 = std::cos(phi_rad) * std::cos(theta_rad) * gz_n_mps2;

  // Aerodynamic forces

  float drag_N = eg_model[utils::to_idx(ModelProperties::CD)] *
                 dynamic_preasure *
                 eg_model[utils::to_idx(ModelProperties::Aref_m2)];
  float side_N = 0;
  float lift_N = 0;

  // External forces
  float Fx_b_N = -(c_alpha * c_beta * drag_N - c_alpha * s_beta * side_N -
                   s_alpha * lift_N);
  float Fy_b_N = -(s_beta * drag_N + c_beta * side_N);
  float Fz_b_N = -(s_alpha * c_beta * drag_N - s_alpha * s_beta * side_N +
                   c_alpha * lift_N);

  // External moments
  float L_b_Nm = 0;
  float M_b_Nm = 0;
  float N_b_Nm = 0;

  // Denominator in roll and yaw rate equations
  float den = Jxx_b_kgm2 * Jzz_b_kgm2 - std::pow(Jxz_b_kgm2, 2);

  // Translational equations
  dx[0] =
      1.0f / m_kg * Fx_b_N + gx_b_mps2 - w_b_mps * q_b_rps + v_b_mps * r_b_rps;
  dx[1] =
      1.0f / m_kg * Fy_b_N + gy_b_mps2 - u_b_mps * r_b_rps + w_b_mps * p_b_rps;
  dx[2] =
      1.0f / m_kg * Fz_b_N + gz_b_mps2 - v_b_mps * p_b_rps + u_b_mps * q_b_rps;

  // Rotational equations

  dx[3] =
      (Jxz_b_kgm2 * (Jxx_b_kgm2 - Jyy_b_kgm2 + Jzz_b_kgm2) * p_b_rps * q_b_rps -
       (Jzz_b_kgm2 * (Jzz_b_kgm2 - Jyy_b_kgm2) + std::pow(Jxz_b_kgm2, 2)) *
           q_b_rps * r_b_rps +
       Jzz_b_kgm2 * L_b_Nm + Jxz_b_kgm2 * N_b_Nm) /
      den;
  dx[4] =
      ((Jzz_b_kgm2 - Jxx_b_kgm2) * r_b_rps * p_b_rps -
       Jxz_b_kgm2 * (std::pow(p_b_rps, 2) - std::pow(r_b_rps, 2)) + M_b_Nm) /
      Jyy_b_kgm2;
  dx[5] = (-Jxz_b_kgm2 * (Jxx_b_kgm2 - Jyy_b_kgm2 + Jzz_b_kgm2) * q_b_rps *
               r_b_rps +
           (Jxx_b_kgm2 * (Jxx_b_kgm2 - Jyy_b_kgm2) + std::pow(Jxz_b_kgm2, 2)) *
               p_b_rps * q_b_rps +
           Jxz_b_kgm2 * L_b_Nm + Jxx_b_kgm2 * N_b_Nm) /
          den;

  // Kinematic equations
  dx[6] = p_b_rps + std::sin(phi_rad) * std::tan(theta_rad) * q_b_rps +
          std::cos(phi_rad) * std::tan(theta_rad) * r_b_rps;
  dx[7] = std::cos(phi_rad) * q_b_rps - std::sin(phi_rad) * r_b_rps;
  dx[8] = std::sin(phi_rad) / std::cos(theta_rad) * q_b_rps +
          std::cos(phi_rad) / std::cos(theta_rad) * r_b_rps;

  // Position equations
  dx[9] = c_theta * c_psi * u_b_mps +
          (-c_phi * s_psi + s_phi * s_theta * c_psi) * v_b_mps +
          (s_phi * s_psi + c_phi * s_theta * c_psi) * w_b_mps;
  dx[10] = c_theta * s_psi * u_b_mps +
           (c_phi * c_psi + s_phi * s_theta * s_psi) * v_b_mps +
           (-s_phi * c_psi + c_phi * s_theta * s_psi) * w_b_mps;
  dx[11] = -s_theta * u_b_mps + s_phi * c_theta * v_b_mps +
           c_phi * c_theta * w_b_mps;

  return dx;
}