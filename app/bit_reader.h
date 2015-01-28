#ifndef BIT_READER_H
#define BIT_READER_H

#include <iterator>
#include "utils.h"

/**
 * A class responsible for bitwise input.
 *
 * @tparam InputIterator an input iterator type for reading a single
 *                       character
 */
template <typename InputIterator = std::istreambuf_iterator<char>>
class BitReader {
 public:
  /**
   * Construct a BitReader from a pair of input iterators.
   *
   * @param[in] begin the iterator referring to the beginning of the sequence to
   *                  read
   * @param[in] end   the iterator referring to past-the-end of the sequence to
   *                  read
   */
  explicit BitReader(const InputIterator& begin,
                     const InputIterator& end = InputIterator())
      : _begin(begin), _end(end), _buffer(0), _count(0) {}

  /**
   * Construct a BitReader.
   *
   * @param[out] source an instance of a class that can be used to
   *                    construct an input iterator (e.g. a container or a
   *                    stream).
   */
  template <typename Source>
  explicit BitReader(Source& source)
      : BitReader(InputIterator(source)) {}

  /**
   * Read @c 8*sizeof(T) bits and store them into @c value.
   *
   * @param[out] value where read bits are stored
   */
  template <typename T>
  void read(T& value);

  /**
   * Read @c bits bits and store them into @c value.
   *
   * @param[out] value where read bits are stored
   * @param      bits  the number of bits to read
   */
  template <typename T>
  void read(T& value, bits_t bits);

  /**
   * Read a single bit and return it.
   *
   * @return the value of the next bit
   */
  bool read();

  /**
   * @return whether or not there are more bits to read
   */
  operator bool() const { return _begin != _end || !empty(); }

  /**
   * @return whether or not there are still bits buffered
   */
  bool empty() const { return _count == 0; }

  /**
   * @return the internal input iterator at its current position
   */
  const InputIterator& next() const { return _begin; }

 private:
  InputIterator _begin;
  InputIterator _end;

  unsigned char _buffer;
  unsigned char _count;
};

template <typename InputIterator>
template <typename T>
inline void BitReader<InputIterator>::read(T& value) {
  if (empty()) {
    value = 0;
    for (decltype(sizeof(T)) i = 0; i < sizeof(T); ++i) {
      (reinterpret_cast<char*>(&value))[i] = *_begin;
      ++_begin;
    }
  } else {
    read(value, 8 * sizeof(T));
  }
}

template <typename InputIterator>
template <typename T>
inline void BitReader<InputIterator>::read(T& value, bits_t bits) {
  value = 0;

  while (bits--) {
    value <<= 1;

    if (read()) {
      ++value;
    }
  }
}

template <typename InputIterator>
inline bool BitReader<InputIterator>::read() {
  if (empty()) {
    _buffer = *_begin;
    ++_begin;
    _count = 7;
  } else {
    --_count;
  }

  bool retval = _buffer & 0x80;

  _buffer <<= 1;

  return retval;
}

#endif /* BIT_READER_H */
