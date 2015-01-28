#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <cstddef>
#include <iterator>
#include <limits>
#include <mutex>
#include <condition_variable>
#include <type_traits>

typedef uint_fast8_t bits_t;

inline constexpr size_t max_size(size_t bits) { return (1 << bits) - 1; }

template <size_t value, typename IntegerType>
constexpr bool fits_in() {
  return value <= std::numeric_limits<IntegerType>::max();
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

/**
 * A struct with a typedef member @c type that represents the smallest
 * signed integer type that fits in the requested number of bits.
 *
 * @tparam max_value the maximum value that the signed type should contain
 */
template <size_t max_value>
using Integer = SmallestInteger<true,
                                fits_in<max_value, int8_t>(),
                                fits_in<max_value, int16_t>(),
                                fits_in<max_value, int32_t>(),
                                fits_in<max_value, int64_t>()>;

/**
 * A struct with a typedef member @c type that represents the smallest
 * unsigned integer type that fits in the requested number of bits.
 *
 * @tparam max_value the maximum value that the unsigned type should contain
 */
template <size_t max_value>
using UnsignedInteger = SmallestInteger<false,
                                        fits_in<max_value, uint8_t>(),
                                        fits_in<max_value, uint16_t>(),
                                        fits_in<max_value, uint32_t>(),
                                        fits_in<max_value, uint64_t>()>;

/**
 * A position/length pair used to encode LZ77/LZSS matches.
 */
template <size_t max_dictionary_size, size_t max_lookahead_buffer_size>
struct Match {
  typename UnsignedInteger<max_dictionary_size>::type position = 0;
  typename UnsignedInteger<max_lookahead_buffer_size>::type length = 0;

  /**
   * Check for a looping match and update the @c length accordingly.
   *
   * @param match_begin a forward iterator referring to the beginning of the
   *                    match in the dictionary
   * @param pattern_begin an input iterator referring to the beginning of the
   *                      lookahead buffer
   * @param pattern_end   an input iterator referring to past-the-end of the
   *                      lookahead buffer
   */
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

/**
 * A thread-safe counter.
 *
 * @tparam T the type of the wrapped counter variable
 */
template <typename T>
class ThreadSafeCounter {
 public:
  /**
   * Construct a TheadSafeCounter.
   *
   * @param value the starting value of the counter
   */
  ThreadSafeCounter(const T& value) : _value(value), _lock(_lock_mutex) {}

  ThreadSafeCounter(const ThreadSafeCounter&) = delete;

  /**
   * @return the value of the counter
   */
  T value() {
    lock_guard_type lock_guard(_mutex);
    return _value;
  }

  /**
   * Increase the value by 1.
   */
  void increase() {
    lock_guard_type lock_guard(_mutex);
    ++_value;
  }

  /**
   * Decrease the value by 1.
   */
  void decrease() {
    lock_guard_type lock_guard(_mutex);
    --_value;
  }

  /**
   * Block the current thread until the counter is notified.
   *
   * @param pred predicate which returns false if the waiting should
   *             be continued
   * @see #notify
   */
  template <typename Predicate>
  void wait(Predicate pred) {
    _condition_variable.wait(_lock, pred);
  }

  /**
   * Notify the counter that its value may have changed.
   */
  void notify() { _condition_variable.notify_one(); }

 private:
  T _value;

  typedef std::mutex mutex_type;
  mutex_type _mutex;

  std::condition_variable _condition_variable;
  mutex_type _lock_mutex;
  std::unique_lock<mutex_type> _lock;

  typedef std::lock_guard<mutex_type> lock_guard_type;
};

/**
 * A functor that casts a value to its unsigned counterpart.
 *
 * @tparam T the type of the values to cast
 */
template <typename T>
struct MakeUnsigned {
  typedef typename std::make_unsigned<T>::type unsigned_type;

  unsigned_type operator()(T value) const {
    return static_cast<unsigned_type>(value);
  }
};

#endif  // UTILS_H
