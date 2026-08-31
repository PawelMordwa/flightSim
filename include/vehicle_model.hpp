#pragma once

#include "utils.hpp"
#include <array>
#include <type_traits>

enum class ModelProperties {
  m_kg,
  Jxz_b_kgm2,
  Jxx_b_kgm2,
  Jyy_b_kgm2,
  Jzz_b_kgm2,
  CD,
  Aref_m2,

  Count
};

extern std::array<float, utils::to_idx(ModelProperties::Count)> eg_model;

void set_bowling_ball_model();