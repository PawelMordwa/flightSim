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
  std::vector<float> x0;

  // Solution
  std::vector<float> t;
  std::vector<std::vector<float>> x;

  Simulation();

private:
  std::vector<float> simulation_set_initial_conditions(void);
};
