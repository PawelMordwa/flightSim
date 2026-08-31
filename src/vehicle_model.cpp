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