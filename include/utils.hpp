#pragma once

#include <type_traits>

namespace utils {
template <typename E> constexpr auto to_idx(E e) {
  return static_cast<std::underlying_type_t<E>>(e);
}
} // namespace utils