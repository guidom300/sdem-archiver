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
  void read(T& value);

  template <typename T>
  void read(T& value, bits_t bits);

  bool read();

  operator bool() const { return _begin != _end || !empty(); }

  bool empty() const { return _count == 0; }

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
