#pragma once

#include "vehicle_model.hpp"
#include <array>
#include <type_traits>
#include <vector>

constexpr int state_vector_size = 12;

class Simulation {
public:
  float t0;
  float tf;
  float ts;
  float nt_s;
  std::vector<float> x0;

  // Solution
  std::vector<float> t;
  std::vector<std::vector<float>> x;

  // tf_s ma wartosc domyslna, wiec stare wywolania Simulation(x0) nadal dzialaja.
  // Wartosc domyslna podaje sie TYLKO w deklaracji, nie w definicji.
  Simulation(std::vector<float> initialState, float tf_s = 50.0f);
};
