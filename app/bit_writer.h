#ifndef BIT_WRITER_H
#define BIT_WRITER_H

#include <iterator>
#include "utils.h"

template <typename OutputIterator = std::ostreambuf_iterator<char>>
class BitWriter {
 public:
  explicit BitWriter(const OutputIterator& output_iterator)
      : _output_iterator(output_iterator), _buffer(0), _count(8) {}

  template <typename Sink>
  explicit BitWriter(Sink& sink)
      : BitWriter(OutputIterator(sink)) {}

  template <typename T>
  void write(T value, bits_t bits = 8 * sizeof(T));

  template <typename Bitset>
  void write_bitset(const Bitset& bitset);

  bool empty() const { return _count == 8; }

  void flush() {
    _buffer <<= _count;
    write_buffer();
  }

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
