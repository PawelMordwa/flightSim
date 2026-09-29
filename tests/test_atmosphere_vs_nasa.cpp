// Walidacja modelu atmosfery (atmosphere::get_density) wzgledem kolumny
// airDensity_slug_ft3 z danych referencyjnych NASA, ktore pochodza z pelnego
// modelu US Standard Atmosphere 1976.
//
// UWAGA: to NIE jest pelny check-case 11. Check-case 11 to lot ustalony F-16
// (baza aerodynamiczna, model napedu, rozwiazanie trymu, WGS-84 + J2), czego
// obecny model nie potrafi odtworzyc. Testujemy tu ten fragment danych z
// check-case'ow 3 i 11, ktory faktycznie pokrywa sie z naszym kodem: gestosc
// powietrza w funkcji wysokosci.
//
// Sciezki do plikow CSV przychodza z CMake jako argv[1..n].

#include "nasa_reference.hpp"

#include "atmosphere.hpp"

#include <cmath>
#include <cstdio>
#include <exception>
#include <string>
#include <vector>

namespace {

constexpr double ft_to_m = 0.3048;
constexpr double slug_ft3_to_kg_m3 = 515.378818;

// Uproszczony wzor barometryczny trzyma sie US-1976 w granicach ~0.3%
// w zakresie wysokosci wystepujacym w tych danych (ok. 3000-9150 m).
constexpr double rtol = 0.005;

bool run(const std::vector<std::string> &reference_paths) {
  double worst_error = 0.0;
  double worst_altitude_m = 0.0;
  double worst_ours = 0.0;
  double worst_nasa = 0.0;
  int samples = 0;

  for (const std::string &path : reference_paths) {
    const nasa::ReferenceData reference(path);
    const std::vector<double> &altitude_ft = reference.column("altitudeMsl_ft");
    const std::vector<double> &density_slug_ft3 =
        reference.column("airDensity_slug_ft3");

    for (std::size_t i = 0; i < altitude_ft.size(); i++) {
      const double altitude_m = altitude_ft[i] * ft_to_m;
      const double nasa_kg_m3 = density_slug_ft3[i] * slug_ft3_to_kg_m3;
      const double ours_kg_m3 =
          atmosphere::get_density(static_cast<float>(altitude_m));

      const double relative_error =
          std::abs(ours_kg_m3 - nasa_kg_m3) / nasa_kg_m3;

      if (relative_error > worst_error) {
        worst_error = relative_error;
        worst_altitude_m = altitude_m;
        worst_ours = ours_kg_m3;
        worst_nasa = nasa_kg_m3;
      }

      samples++;
    }
  }

  if (samples == 0) {
    std::printf("BLAD: brak probek do porownania\n");
    return false;
  }

  std::printf("Porownano %d probek gestosci, najgorszy blad %.3f%% "
              "na %.1f m (nasze %.5f, NASA %.5f kg/m3)\n",
              samples, worst_error * 100.0, worst_altitude_m, worst_ours,
              worst_nasa);

  if (worst_error > rtol) {
    std::printf("NIEZGODNOSC: przekroczono tolerancje %.1f%%\n", rtol * 100.0);
    return false;
  }

  return true;
}

} // namespace

int main(int argc, char **argv) {
  if (argc < 2) {
    std::printf("Uzycie: %s <sciezka/do/pliku.csv> [kolejne pliki...]\n",
                argv[0]);
    return 2;
  }

  const std::vector<std::string> paths(argv + 1, argv + argc);

  try {
    return run(paths) ? 0 : 1;
  } catch (const std::exception &error) {
    std::printf("Wyjatek: %s\n", error.what());
    return 2;
  }
}
