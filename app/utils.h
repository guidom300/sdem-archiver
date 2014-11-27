#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <cstddef>

typedef uint_fast8_t bits_t;

static inline constexpr size_t max_size(size_t bits) { return (1 << bits) - 1; }

#endif  // UTILS_H
