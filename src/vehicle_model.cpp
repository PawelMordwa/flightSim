#include "vehicle_model.hpp"
#include "utils.hpp"
#include <array>
#include <cmath>
#include <iostream>
#include <type_traits>

std::array<float, utils::to_idx(ModelProperties::Count)> eg_model{{0}};

void set_bowling_ball_model() {
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
  eg_model[utils::to_idx(ModelProperties::CD)] = CD;
  eg_model[utils::to_idx(ModelProperties::Aref_m2)] = Aref_m2;
}

void set_brick_model() {
  const float m_kg = 2.26796f;
  const float CD = 0.0f;
  const float Jxx_b_kgm2 = 0.0025680f;
  const float Jyy_b_kgm2 = 0.0084213f;
  const float Jzz_b_kgm2 = 0.0097545f;
  const float Jxz_b_kgm2 = 0.0f;

  float Aref_m2 = 0; // insignificant since CD = 0

  eg_model[utils::to_idx(ModelProperties::m_kg)] = m_kg;
  eg_model[utils::to_idx(ModelProperties::Jxx_b_kgm2)] = Jxx_b_kgm2;
  eg_model[utils::to_idx(ModelProperties::Jyy_b_kgm2)] = Jyy_b_kgm2;
  eg_model[utils::to_idx(ModelProperties::Jzz_b_kgm2)] = Jzz_b_kgm2;
  eg_model[utils::to_idx(ModelProperties::Jxz_b_kgm2)] = Jxz_b_kgm2;
  eg_model[utils::to_idx(ModelProperties::CD)] = CD;
  eg_model[utils::to_idx(ModelProperties::Aref_m2)] = Aref_m2;
}