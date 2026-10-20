#pragma once

#include "vehicle_model.hpp"
struct AeroForces {
  float drag_N;
  float side_N;
  float lift_N;
};

struct BodyForces {
  float Fx_N;
  float Fy_N;
  float Fz_N;
};

struct BodyMoments {
  float L_b_Nm;
  float M_b_Nm;
  float N_b_Nm;
};

struct AeroCoeficients {
  float CD;
  float CY;
  float CL;
  float Cl;
  float Cm;
  float Cn;
};

struct LinearAeroModel {
  float CD0, CL0, CL_alpha, CY_beta;
  float Cl_beta, Cl_p, Cm0, Cm_alpha, Cm_q, Cn_beta, Cn_r;
};

// Wypelniany przez set_*_model() w vehicle_model.cpp.
extern LinearAeroModel eg_aero_model;

AeroCoeficients aero_coeficients(const LinearAeroModel &aero_model,
                                 float alpha_rad, float beta_rad, float p_hat,
                                 float q_hat, float r_hat);

AeroForces aero_forces(float q_bar, float Aref_m2, float CD, float CY,
                       float CL);

BodyForces wind_to_body(AeroForces f, float alpha_rad, float beta_rad);

BodyMoments external_moments(const AeroCoeficients &aero_coeficients,
                             const ModelArray &model, float q_bar);
