#pragma once

#include <cmath>
#include <type_traits>

namespace atmosphere {
constexpr float rho_0 = 1.2255;

// todo: dodać bardziej ogólny wzór dla całej atmosfery
float get_density(float h_m) { return rho_0 * pow((1 - (h_m / 44300)), 4.256); }
} // namespace atmosphere