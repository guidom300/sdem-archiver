#ifndef BIT_READER_H
#define BIT_READER_H

#include <iterator>
#include "utils.h"

template <typename InputIterator = std::istreambuf_iterator<char>>
class BitReader {
 public:
  explicit BitReader(const InputIterator& begin,
                     const InputIterator& end = InputIterator())
      : _begin(begin), _end(end), _buffer(0), _count(0) {}

  template <typename Source>
  explicit BitReader(Source& source)
      : BitReader(InputIterator(source)) {}

  template <typename T>
  T read(bits_t bits = 8 * sizeof(T));

  operator bool() const { return _begin != _end; }

  bool empty() const { return _count == 0; }

 private:
  InputIterator _begin;
  InputIterator _end;

  unsigned char _buffer;
  unsigned char _count;
};

template <typename InputIterator>
template <typename T>
inline T BitReader<InputIterator>::read(bits_t bits) {
  T value = 0;

  while (bits--) {
    value <<= 1;

    if (empty()) {
      _buffer = *_begin;
      ++_begin;
      _count = 8;
    }

    if (_buffer & 0x80) {
      ++value;
    }

    --_count;
    _buffer <<= 1;
  }

  return value;
}

#endif /* BIT_READER_H */
