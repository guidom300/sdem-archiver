#ifndef BIT_WRITER_H
#define BIT_WRITER_H

#include <iterator>
#include "utils.h"

template <typename OutputIterator = std::ostreambuf_iterator<char>>
class BitWriter {
 public:
  explicit BitWriter(const OutputIterator &output_iterator)
      : _output_iterator(output_iterator), _buffer(0), _count(8) {}

  template <typename Sink>
  explicit BitWriter(Sink &sink)
      : BitWriter(OutputIterator(sink)) {}

  template <typename V>
  void write(V value, bits_t bits = 8 * sizeof(V));

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
};

template <typename OutputIterator>
template <typename V>
inline void BitWriter<OutputIterator>::write(V value, bits_t bits) {
  static constexpr V mask = V(1) << (8 * sizeof(V) - 1);

  value <<= 8 * sizeof(V) - bits;

  while (bits--) {
    _buffer <<= 1;

    if (value & mask) {
      ++_buffer;
    }

    value <<= 1;

    if (--_count == 0) {
      write_buffer();
    }
  }
}

#endif /* BIT_WRITER_H */
