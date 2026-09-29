#pragma once

#include <map>
#include <string>
#include <vector>

namespace nasa {

// Wczytuje plik CSV z danymi referencyjnymi NASA (NESC-RP-12-00770) i udostepnia
// kolumny po nazwie z naglowka. Wszystkie wartosci sa w jednostkach US
// (ft, deg, slug, lbf) - konwersja do SI nalezy do kodu testu.
class ReferenceData {
public:
  explicit ReferenceData(const std::string &path);

  bool has(const std::string &column_name) const;

  // Rzuca std::runtime_error jesli kolumna nie istnieje.
  const std::vector<double> &column(const std::string &column_name) const;

  std::size_t rows() const { return rows_; }
  const std::string &path() const { return path_; }

private:
  std::string path_;
  std::map<std::string, std::vector<double>> columns_;
  std::size_t rows_ = 0;
};

// Indeks wiersza, ktorego "time" jest najblizszy zadanej chwili.
std::size_t nearest_time_index(const std::vector<double> &time_s, double t_s);

} // namespace nasa
