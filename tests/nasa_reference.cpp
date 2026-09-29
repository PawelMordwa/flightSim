#include "nasa_reference.hpp"

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace nasa {
namespace {

// Rozbija jedna linie CSV na pola. Pliki NASA nie zawieraja cudzyslowow
// ani przecinkow wewnatrz pol, wiec wystarczy prosty podzial.
std::vector<std::string> split_line(const std::string &line) {
  std::vector<std::string> fields;
  std::istringstream stream(line);
  std::string field;

  while (std::getline(stream, field, ',')) {
    fields.push_back(field);
  }

  return fields;
}

} // namespace

ReferenceData::ReferenceData(const std::string &path) : path_(path) {
  std::ifstream file(path);

  if (!file.is_open()) {
    throw std::runtime_error("Nie mozna otworzyc pliku referencyjnego: " + path);
  }

  std::string line;

  if (!std::getline(file, line)) {
    throw std::runtime_error("Pusty plik referencyjny: " + path);
  }

  const std::vector<std::string> header = split_line(line);

  if (header.empty()) {
    throw std::runtime_error("Brak naglowka w pliku referencyjnym: " + path);
  }

  std::vector<std::vector<double>> values(header.size());

  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    const std::vector<std::string> fields = split_line(line);

    if (fields.size() != header.size()) {
      throw std::runtime_error("Niezgodna liczba kolumn w pliku " + path +
                               ", wiersz " + std::to_string(rows_ + 2));
    }

    for (std::size_t i = 0; i < fields.size(); i++) {
      values[i].push_back(std::stod(fields[i]));
    }

    rows_++;
  }

  for (std::size_t i = 0; i < header.size(); i++) {
    columns_[header[i]] = std::move(values[i]);
  }
}

bool ReferenceData::has(const std::string &column_name) const {
  return columns_.find(column_name) != columns_.end();
}

const std::vector<double> &
ReferenceData::column(const std::string &column_name) const {
  const auto it = columns_.find(column_name);

  if (it == columns_.end()) {
    throw std::runtime_error("Brak kolumny '" + column_name + "' w pliku " +
                             path_);
  }

  return it->second;
}

std::size_t nearest_time_index(const std::vector<double> &time_s, double t_s) {
  std::size_t best = 0;
  double best_distance = std::abs(time_s.at(0) - t_s);

  for (std::size_t i = 1; i < time_s.size(); i++) {
    const double distance = std::abs(time_s[i] - t_s);

    if (distance < best_distance) {
      best_distance = distance;
      best = i;
    }
  }

  return best;
}

} // namespace nasa
