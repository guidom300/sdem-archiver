#ifndef BIT_WRITER_H
#define BIT_WRITER_H

#include <iterator>
#include "utils.h"

/**
 * A class responsible for bitwise output.
 *
 * @tparam OutputIterator an output iterator type for writing a single
 *                        character
 */
template <typename OutputIterator = std::ostreambuf_iterator<char>>
class BitWriter {
 public:
  /**
   * Construct a BitWriter.
   *
   * @param[out] output_iterator an output iterator
   */
  explicit BitWriter(const OutputIterator& output_iterator)
      : _output_iterator(output_iterator), _buffer(0), _count(8) {}

  /**
   * Construct a BitWriter.
   *
   * @param[out] sink an instance of a class that can be used to construct
   *                  an output iterator (e.g. a container or a stream).
   */
  template <typename Sink>
  explicit BitWriter(Sink& sink)
      : BitWriter(OutputIterator(sink)) {}

  /**
   * Write all bits of @c value.
   *
   * @param value the value to write
   */
  template <typename T>
  void write(T value);

  /**
   * Write the least significant @c bits bits of value.
   *
   * @param value the value to read from
   * @param bits  the number of least significant bits to write
   */
  template <typename T>
  void write(T value, bits_t bits);

  /**
   * Write a sequence of bits.
   *
   * @param[in] bitset a container of @c bool values
   */
  template <typename Bitset>
  void write_bitset(const Bitset& bitset);

  /**
   * @return whether or not there are still bits to be written
   */
  bool empty() const { return _count == 8; }

  /**
   * Empty the internal buffer, padding it if necessary.
   */
  void flush() {
    _buffer <<= _count;
    write_buffer();
  }

  /**
   * Flush the BitWriter on destruction.
   */
  ~BitWriter() {
    if (!empty()) {
      flush();
    }
  }

 private:
  OutputIterator _output_iterator;

  unsigned char _buffer;
  unsigned char _count;

  void write_buffer() {
    _output_iterator = _buffer;
    _buffer = 0;
    _count = 8;
  }

  void process(bool bit);
};

template <typename OutputIterator>
template <typename T>
inline void BitWriter<OutputIterator>::write(T value) {
  if (empty()) {
    for (decltype(sizeof(T)) i = 0; i < sizeof(T); ++i) {
      _output_iterator = (reinterpret_cast<char*>(&value))[i];
    }
  } else {
    write(value, 8 * sizeof(T));
  }
}

template <typename OutputIterator>
template <typename T>
inline void BitWriter<OutputIterator>::write(T value, bits_t bits) {
  static constexpr T mask = T(1) << (8 * sizeof(T) - 1);

  value <<= 8 * sizeof(T) - bits;

  while (bits--) {
    process(value & mask);

    value <<= 1;
  }
}

template <typename OutputIterator>
template <typename Bitset>
inline void BitWriter<OutputIterator>::write_bitset(const Bitset& bitset) {
  for (auto&& bit : bitset) {
    process(bit);
  }
}

template <typename OutputIterator>
inline void BitWriter<OutputIterator>::process(bool bit) {
  _buffer <<= 1;

  if (bit) {
    ++_buffer;
  }

  if (--_count == 0) {
    write_buffer();
  }
}

#endif /* BIT_WRITER_H */
