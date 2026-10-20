#include "aerodynamics.hpp"
#include <algorithm>
#include <cmath>

LinearAeroModel eg_aero_model = {};

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

AeroCoeficients aero_coeficients(const LinearAeroModel &aero_model,
                                 float alpha_rad, float beta_rad, float p_hat,
                                 float q_hat, float r_hat) {
  AeroCoeficients coef = {};

  coef.CD = aero_model.CD0;
  coef.CY = aero_model.CY_beta * beta_rad;
  coef.CL = aero_model.CL0 + (aero_model.CL_alpha * alpha_rad);
  coef.Cl = aero_model.Cl_beta * beta_rad + (aero_model.Cl_p * p_hat);
  coef.Cm = aero_model.Cm0 + (aero_model.Cm_alpha * alpha_rad) +
            (aero_model.Cm_q * q_hat);
  coef.Cn = aero_model.Cn_beta * beta_rad + (aero_model.Cn_r * r_hat);

  return coef;
};

// p_hat/q_hat/r_hat sa juz zawarte w Cl/Cm/Cn (aero_coeficients), wiec tu
// zostaje tylko wymiarowanie: M = C * q_bar * S * l_ref.
BodyMoments external_moments(const AeroCoeficients &aero_coeficients,
                             const ModelArray &model, float q_bar) {
  BodyMoments M = {};
  M.L_b_Nm = aero_coeficients.Cl * q_bar *
             model[utils::to_idx(ModelProperties::Aref_m2)] *
             model[utils::to_idx(ModelProperties::b_m)];
  M.M_b_Nm = aero_coeficients.Cm * q_bar *
             model[utils::to_idx(ModelProperties::Aref_m2)] *
             model[utils::to_idx(ModelProperties::c_m)];
  M.N_b_Nm = aero_coeficients.Cn * q_bar *
             model[utils::to_idx(ModelProperties::Aref_m2)] *
             model[utils::to_idx(ModelProperties::b_m)];

  return M;
}
