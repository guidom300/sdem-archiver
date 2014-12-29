#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <cstddef>
#include <iterator>
#include <limits>

typedef uint_fast8_t bits_t;

inline constexpr size_t max_size(size_t bits) { return (1 << bits) - 1; }

template <size_t value, typename IntegerType>
constexpr bool fits_in() {
  return value < std::numeric_limits<IntegerType>::max();
}

template <bool Signed,
          bool FitsIn8Bits = false,
          bool FitsIn16Bits = FitsIn8Bits,
          bool FitsIn32Bits = FitsIn16Bits,
          bool FitsIn64Bits = FitsIn32Bits>
struct SmallestInteger;

template <>
struct SmallestInteger<true, true> {
  typedef int8_t type;
};

template <>
struct SmallestInteger<false, true> {
  typedef uint8_t type;
};

template <>
struct SmallestInteger<true, false, true> {
  typedef int16_t type;
};

template <>
struct SmallestInteger<false, false, true> {
  typedef uint16_t type;
};

template <>
struct SmallestInteger<true, false, false, true> {
  typedef int32_t type;
};

template <>
struct SmallestInteger<false, false, false, true> {
  typedef uint32_t type;
};

template <>
struct SmallestInteger<true, false, false, false, true> {
  typedef int64_t type;
};

template <>
struct SmallestInteger<false, false, false, false, true> {
  typedef uint64_t type;
};

template <size_t max_value>
using Integer = SmallestInteger<true,
                                fits_in<max_value, int8_t>(),
                                fits_in<max_value, int16_t>(),
                                fits_in<max_value, int32_t>(),
                                fits_in<max_value, int64_t>()>;

template <size_t max_value>
using UnsignedInteger = SmallestInteger<false,
                                        fits_in<max_value, uint8_t>(),
                                        fits_in<max_value, uint16_t>(),
                                        fits_in<max_value, uint32_t>(),
                                        fits_in<max_value, uint64_t>()>;

template <size_t max_dictionary_size, size_t max_lookahead_buffer_size>
struct Match {
  typename UnsignedInteger<max_dictionary_size>::type position = 0;
  typename UnsignedInteger<max_lookahead_buffer_size>::type length = 0;

  template <typename ForwardIterator, typename InputIterator>
  void check_repetition(ForwardIterator match_begin,
                        InputIterator pattern_begin,
                        InputIterator pattern_end) {
    if (length > 0 && position == length) {
      std::advance(pattern_begin, length);

      decltype(length) count = 0;

      for (auto current = match_begin;
           pattern_begin != pattern_end && *current == *pattern_begin;
           ++pattern_begin) {
        ++current;
        if (++count % length == 0) {
          current = match_begin;
        }
      }

      length += count;
    }
  }
};

#endif  // UTILS_H
