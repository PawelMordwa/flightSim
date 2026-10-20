#pragma once

#include "utils.hpp"
#include <array>
#include <type_traits>

// Masa, bezwladnosc i geometria pojazdu. Wspolczynniki aerodynamiczne
// (pochodne CL_alpha, Cm_q, ...) zyja osobno w LinearAeroModel
// (aerodynamics.hpp) - jedno zrodlo prawdy dla kazdej wielkosci.
enum class ModelProperties {
  m_kg,
  Jxz_b_kgm2,
  Jxx_b_kgm2,
  Jyy_b_kgm2,
  Jzz_b_kgm2,
  Aref_m2,
  b_m,
  c_m,

  Count,
};

using ModelArray = std::array<float, utils::to_idx(ModelProperties::Count)>;

extern ModelArray eg_model;

void set_bowling_ball_model();

void set_brick_model(bool with_dumping);