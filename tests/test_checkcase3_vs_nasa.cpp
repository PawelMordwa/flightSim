// NASA NESC check-case 3: "Tumbling brick with dynamic damping, no drag".
// Porownuje predkosci katowe p/q/r naszej symulacji z danymi referencyjnymi
// jednej z symulacji uczestniczacych w badaniu NESC-RP-12-00770.
//
// Sciezka do pliku CSV przychodzi z CMake jako argv[1] - patrz tests/CMakeLists.txt.

#include "nasa_reference.hpp"

#include "numerical_integration.hpp"
#include "simulation_setup.hpp"
#include "vehicle_model.hpp"

#include <cmath>
#include <cstdio>
#include <exception>
#include <string>
#include <vector>

namespace {

constexpr double ft_to_m = 0.3048;
constexpr double deg_per_rad = 180.0 / M_PI;

// Scenariusz z Tabeli 27 dokumentu NESC: start z 30 000 ft, zerowa predkosc
// liniowa, predkosci katowe 10/20/30 deg/s wzgledem ukladu inercjalnego.
constexpr double start_altitude_ft = 30000.0;
constexpr double duration_s = 30.0;

// Tolerancja hybrydowa: |ours - nasa| <= atol + rtol * |nasa|.
// Sam warunek wzglednny nie wystarcza, bo predkosci katowe gasna do zera,
// a wtedy blad wzgledny rosnie do 100% mimo znikomego bledu bezwzglednego.
constexpr double atol_deg_s = 0.05;
constexpr double rtol = 0.02;

struct Comparison {
  double worst_excess = 0.0; // o ile przekroczono tolerancje (0 = w normie)
  double worst_time_s = 0.0;
  double worst_ours = 0.0;
  double worst_nasa = 0.0;
  const char *worst_axis = "-";
  int samples = 0;
};

void check_axis(Comparison &c, const char *axis, double t_s, double ours_deg_s,
                double nasa_deg_s) {
  const double tolerance = atol_deg_s + rtol * std::abs(nasa_deg_s);
  const double error = std::abs(ours_deg_s - nasa_deg_s);
  const double excess = error - tolerance;

  if (excess > c.worst_excess) {
    c.worst_excess = excess;
    c.worst_time_s = t_s;
    c.worst_ours = ours_deg_s;
    c.worst_nasa = nasa_deg_s;
    c.worst_axis = axis;
  }
}

bool run(const std::string &reference_path) {
  const nasa::ReferenceData reference(reference_path);

  set_brick_model(true);

  const std::vector<float> initial_state = {
      0.0f,
      0.0f,
      0.0f,
      static_cast<float>(10.0 / deg_per_rad),
      static_cast<float>(20.0 / deg_per_rad),
      static_cast<float>(30.0 / deg_per_rad),
      0.0f,
      0.0f,
      0.0f,
      0.0f,
      0.0f,
      static_cast<float>(-start_altitude_ft * ft_to_m)};

  Simulation sim(initial_state, static_cast<float>(duration_s));
  forward_euler(sim);

  const std::vector<double> &time_s = reference.column("time");
  const std::vector<double> &roll = // p
      reference.column("bodyAngularRateWrtEi_deg_s_Roll");
  const std::vector<double> &pitch = // q
      reference.column("bodyAngularRateWrtEi_deg_s_Pitch");
  const std::vector<double> &yaw = // r
      reference.column("bodyAngularRateWrtEi_deg_s_Yaw");

  const std::size_t steps = static_cast<std::size_t>(sim.nt_s);
  Comparison comparison;

  for (std::size_t i = 0; i < time_s.size(); i++) {
    if (time_s[i] > duration_s) {
      continue;
    }

    // Nasz krok calkowania (0.005 s) jest gestszy niz probkowanie NASA (0.1 s),
    // wiec wybieramy najblizszy krok wlasnej symulacji.
    const std::size_t k =
        static_cast<std::size_t>(std::lround(time_s[i] / sim.ts));

    if (k >= steps) {
      continue;
    }

    check_axis(comparison, "p", time_s[i], sim.x[3][k] * deg_per_rad, roll[i]);
    check_axis(comparison, "q", time_s[i], sim.x[4][k] * deg_per_rad, pitch[i]);
    check_axis(comparison, "r", time_s[i], sim.x[5][k] * deg_per_rad, yaw[i]);
    comparison.samples++;
  }

  if (comparison.samples == 0) {
    std::printf("BLAD: zadna probka referencyjna nie zostala porownana (%s)\n",
                reference_path.c_str());
    return false;
  }

  if (comparison.worst_excess > 0.0) {
    std::printf("NIEZGODNOSC z %s\n", reference_path.c_str());
    std::printf("  os %s w t = %.2f s: nasze = %.6f deg/s, NASA = %.6f deg/s\n",
                comparison.worst_axis, comparison.worst_time_s,
                comparison.worst_ours, comparison.worst_nasa);
    std::printf("  przekroczenie tolerancji o %.6f deg/s "
                "(atol = %.3f, rtol = %.3f)\n",
                comparison.worst_excess, atol_deg_s, rtol);
    return false;
  }

  std::printf("OK: %d probek zgodnych z %s (atol = %.3f deg/s, rtol = %.1f%%)\n",
              comparison.samples, reference_path.c_str(), atol_deg_s,
              rtol * 100.0);
  return true;
}

} // namespace

int main(int argc, char **argv) {
  if (argc < 2) {
    std::printf("Uzycie: %s <sciezka/do/Atmos_03_sim_XX.csv>\n", argv[0]);
    return 2;
  }

  try {
    return run(argv[1]) ? 0 : 1;
  } catch (const std::exception &error) {
    std::printf("Wyjatek: %s\n", error.what());
    return 2;
  }
}
