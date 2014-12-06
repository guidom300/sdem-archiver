#ifndef LZ77_ENCODER_H
#define LZ77_ENCODER_H

#include <deque>
#include "utils.h"
#include "bit_writer.h"
#include "lz77_boyer_moore_dictionary.h"

template <
    bits_t position_bits,
    bits_t length_bits,
    size_t max_dictionary_size = max_size(position_bits),
    size_t max_lookahead_buffer_size = max_size(length_bits),
    typename T = char,
    template <size_t, size_t, typename> class D = LZ77BoyerMooreDictionary>
class LZ77Encoder {
 public:
  typedef T symbol_type;
  typedef std::deque<T> lookahead_buffer_type;
  typedef D<max_dictionary_size, max_lookahead_buffer_size, T> dictionary_type;

  template <typename InputIterator, typename OutputIterator>
  size_t operator()(InputIterator begin,
                    InputIterator end,
                    OutputIterator output_iterator);

  void clear();

 private:
  typedef typename dictionary_type::match_type match_type;
  lookahead_buffer_type _lookahead_buffer;
  dictionary_type _dictionary;

  void slide_window(size_t length);
};

template <bits_t position_bits,
          bits_t length_bits,
          size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T,
          template <size_t, size_t, typename> class D>
template <typename InputIterator, typename OutputIterator>
size_t LZ77Encoder<position_bits,
                   length_bits,
                   max_dictionary_size,
                   max_lookahead_buffer_size,
                   T,
                   D>::
operator()(InputIterator begin,
           InputIterator end,
           OutputIterator output_iterator) {
  BitWriter<OutputIterator> bit_writer(output_iterator);
  size_t steps = 0;

  while (begin != end || !_lookahead_buffer.empty()) {
    // Fill lookahead buffer
    for (; begin != end && _lookahead_buffer.size() < max_lookahead_buffer_size;
         ++begin) {
      _lookahead_buffer.push_back(*begin);
    }

    // Find a match
    match_type match = _dictionary.find_match(_lookahead_buffer.cbegin(),
                                              _lookahead_buffer.cend());

    // Get the next character
    if (match.length == _lookahead_buffer.size()) {
      if (begin != end) {
        _lookahead_buffer.push_back(*begin);
        ++begin;
      } else {
        --match.length;
      }
    }

    symbol_type next_symbol = _lookahead_buffer[match.length];

    // Write output
    bit_writer.write(match.position, position_bits);
    bit_writer.write(match.length, length_bits);
    bit_writer.write(next_symbol);

    // Move window
    slide_window(match.length + 1);

    // Keep count of the number of steps for decoding
    ++steps;
  }

  return steps;
}

template <bits_t position_bits,
          bits_t length_bits,
          size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T,
          template <size_t, size_t, typename> class D>
inline void LZ77Encoder<position_bits,
                        length_bits,
                        max_dictionary_size,
                        max_lookahead_buffer_size,
                        T,
                        D>::clear() {
  _lookahead_buffer.clear();
  _dictionary.clear();
}

template <bits_t position_bits,
          bits_t length_bits,
          size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T,
          template <size_t, size_t, typename> class D>
inline void LZ77Encoder<position_bits,
                        length_bits,
                        max_dictionary_size,
                        max_lookahead_buffer_size,
                        T,
                        D>::slide_window(size_t length) {
  auto begin = _lookahead_buffer.begin();
  auto end = begin + length;
  _dictionary.slide_window(begin, end);
  _lookahead_buffer.erase(begin, end);
}

#endif /* LZ77_ENCODER_H */
