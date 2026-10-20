#include "vehicle_model.hpp"
#include "aerodynamics.hpp"
#include "utils.hpp"
#include <array>
#include <cmath>
#include <iostream>
#include <type_traits>

ModelArray eg_model{{}};

void set_bowling_ball_model() {
  eg_model.fill(0.0f);
  eg_aero_model = {};

  const float m_kg = 6.0f;
  const float CD = 0.47f;
  const float r_m = 0.1085f;

  float J = 2.0f / 5.0f * m_kg * pow(r_m, 2);
  float Aref_m2 = M_PI * pow(r_m, 2);

  eg_model[utils::to_idx(ModelProperties::m_kg)] = m_kg;
  eg_model[utils::to_idx(ModelProperties::Jxz_b_kgm2)] = 0.0f;
  eg_model[utils::to_idx(ModelProperties::Jxx_b_kgm2)] = J;
  eg_model[utils::to_idx(ModelProperties::Jyy_b_kgm2)] = J;
  eg_model[utils::to_idx(ModelProperties::Jzz_b_kgm2)] = J;
  eg_model[utils::to_idx(ModelProperties::Aref_m2)] = Aref_m2;

  eg_aero_model.CD0 = CD;
}

void set_brick_model(bool with_dumping) {
  eg_model.fill(0.0f);
  eg_aero_model = {};

  const float m_kg = 2.26796f;
  const float CD = 0.0f;
  const float Jxx_b_kgm2 = 0.0025680f;
  const float Jyy_b_kgm2 = 0.0084213f;
  const float Jzz_b_kgm2 = 0.0097545f;
  const float Jxz_b_kgm2 = 0.0f;
  const float b_m = 0.1016f;
  const float c_m = 0.2032f;

  float Cl_p = -1.0f;
  float Cm_q = -1.0f;
  float Cn_r = -1.0f;

  if (with_dumping == false) {
    Cl_p = 0.0f;
    Cm_q = 0.0f;
    Cn_r = 0.0f;
  }

  float Aref_m2 = b_m * c_m;

  eg_model[utils::to_idx(ModelProperties::m_kg)] = m_kg;
  eg_model[utils::to_idx(ModelProperties::Jxx_b_kgm2)] = Jxx_b_kgm2;
  eg_model[utils::to_idx(ModelProperties::Jyy_b_kgm2)] = Jyy_b_kgm2;
  eg_model[utils::to_idx(ModelProperties::Jzz_b_kgm2)] = Jzz_b_kgm2;
  eg_model[utils::to_idx(ModelProperties::Jxz_b_kgm2)] = Jxz_b_kgm2;
  eg_model[utils::to_idx(ModelProperties::Aref_m2)] = Aref_m2;
  eg_model[utils::to_idx(ModelProperties::b_m)] = b_m;
  eg_model[utils::to_idx(ModelProperties::c_m)] = c_m;

  eg_aero_model.CD0 = CD;
  eg_aero_model.Cl_p = Cl_p;
  eg_aero_model.Cm_q = Cm_q;
  eg_aero_model.Cn_r = Cn_r;
}