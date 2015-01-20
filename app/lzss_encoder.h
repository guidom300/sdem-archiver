#ifndef LZSS_ENCODER_H
#define LZSS_ENCODER_H

#include <cmath>
#include "lz77_encoder.h"

static constexpr bool LZSS_ENCODED_FLAG = false;
static constexpr bool LZSS_UNENCODED_FLAG = true;

template <
    bits_t position_bits,
    bits_t length_bits,
    size_t minimum_match_length = (position_bits + length_bits) / 9 + 1,
    size_t max_dictionary_size = max_size(position_bits),
    size_t max_lookahead_buffer_size =
        max_size(length_bits) + minimum_match_length,
    typename T = char,
    template <size_t, size_t, typename> class D = LZ77BoyerMooreDictionary>
class LZSSEncoder : public LZ77Encoder<position_bits,
                                       length_bits,
                                       max_dictionary_size,
                                       max_lookahead_buffer_size,
                                       T,
                                       D> {
 public:
  template <typename InputIterator, typename OutputIterator>
  size_t operator()(InputIterator begin,
                    InputIterator end,
                    OutputIterator output_iterator);
};

template <bits_t position_bits,
          bits_t length_bits,
          size_t minimum_match_length = (position_bits + length_bits) / 9 + 2,
          size_t max_dictionary_size = max_size(position_bits),
          size_t max_lookahead_buffer_size = max_size(length_bits),
          typename T = char>
using NaiveLZSSEncoder = LZSSEncoder<position_bits,
                                     length_bits,
                                     minimum_match_length,
                                     max_dictionary_size,
                                     max_lookahead_buffer_size,
                                     T,
                                     LZ77NaiveDictionary>;

template <bits_t position_bits,
          bits_t length_bits,
          size_t minimum_match_length,
          size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T,
          template <size_t, size_t, typename> class D>
template <typename InputIterator, typename OutputIterator>
size_t LZSSEncoder<position_bits,
                   length_bits,
                   minimum_match_length,
                   max_dictionary_size,
                   max_lookahead_buffer_size,
                   T,
                   D>::
operator()(InputIterator begin,
           InputIterator end,
           OutputIterator output_iterator) {
  BitWriter<OutputIterator> bit_writer(output_iterator);
  size_t steps = 0;

  while (begin != end || !this->_lookahead_buffer.empty()) {
    this->fill_lookahead_buffer(begin, end);

    // Find a match
    auto match = this->_dictionary.find_match(this->_lookahead_buffer.cbegin(),
                                              this->_lookahead_buffer.cend());

    // Decide whether to encode it or not
    if (match.length < minimum_match_length) {
      bit_writer.write(LZSS_UNENCODED_FLAG, 1);
      bit_writer.write(this->_lookahead_buffer[0]);
      this->slide_window(1);
    } else {
      bit_writer.write(LZSS_ENCODED_FLAG, 1);
      bit_writer.write(match.position, position_bits);
      bit_writer.write(match.length - minimum_match_length, length_bits);
      this->slide_window(match.length);
    }

    // Keep count of the number of steps for decoding
    ++steps;
  }

  return steps;
}

#endif /* LZSS_ENCODER_H */
