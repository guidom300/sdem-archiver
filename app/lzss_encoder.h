#ifndef LZSS_ENCODER_H
#define LZSS_ENCODER_H

#include "lz77_data.h"
#include "bit_writer.h"
#include "lz77_boyer_moore_dictionary.h"
#include "lz77_naive_dictionary.h"

static constexpr bool LZSS_ENCODED_FLAG = false;
static constexpr bool LZSS_UNENCODED_FLAG = true;

constexpr size_t get_minimum_match_length(bits_t position_bits,
                                          bits_t length_bits) {
  return (position_bits + length_bits) / 9 + 1;
}

/**
 * A functor that encodes a sequence of symbols using LZSS.
 *
 * @tparam position_bits the number of bits used to encode the match position
 * @tparam length_bits   the number of bits used to encode the match length
 * @tparam minimum_match_length the minimum match length to accept
 * @tparam max_dictionary_size  the maximum number of symbols that the
 *                              dictionary may contain
 * @tparam max_lookahead_buffer_size the maximum number of symbols that the
 *                                   lookahead buffer may contain
 * @tparam T the type of the symbols
 * @tparam D the type of the matching algorithm to use
 */
template <
    bits_t position_bits,
    bits_t length_bits,
    size_t minimum_match_length =
        get_minimum_match_length(position_bits, length_bits),
    size_t max_dictionary_size = max_size(position_bits),
    size_t max_lookahead_buffer_size =
        max_size(length_bits) + minimum_match_length,
    typename T = char,
    template <size_t, size_t, typename> class D = LZ77BoyerMooreDictionary>
class LZSSEncoder {
 public:
  typedef T symbol_type;
  typedef D<max_dictionary_size, max_lookahead_buffer_size, T> dictionary_type;

  /**
   * Encode a sequence of symbols.
   *
   * @param begin an input iterator referring to the beginning of the sequence
   *              to encode
   * @param end   an input iterator referring to past-the-end of the sequence to
   *              encode
   * @param output_iterator an output iterator for writing the encoded sequence
   * @return the number of steps for decoding
   */
  template <typename InputIterator, typename OutputIterator>
  size_t operator()(InputIterator begin,
                    InputIterator end,
                    OutputIterator output_iterator) {
    typename DataType<max_dictionary_size,
                      max_lookahead_buffer_size,
                      InputIterator,
                      T>::value data(begin, end);
    return encode(data, output_iterator);
  }

 private:
  template <typename Data, typename OutputIterator>
  size_t encode(Data& data, OutputIterator output_iterator);
};

template <bits_t position_bits,
          bits_t length_bits,
          size_t minimum_match_length =
              get_minimum_match_length(position_bits, length_bits),
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
template <typename Data, typename OutputIterator>
size_t LZSSEncoder<position_bits,
                   length_bits,
                   minimum_match_length,
                   max_dictionary_size,
                   max_lookahead_buffer_size,
                   T,
                   D>::encode(Data& data, OutputIterator output_iterator) {
  dictionary_type dictionary;
  BitWriter<OutputIterator> bit_writer(output_iterator);
  size_t steps = 0;

  while (!data.empty()) {
    data.fill_lookahead_buffer();

    // Find a match
    auto match = dictionary.find_match(data);

    // Decide whether to encode it or not
    if (match.length < minimum_match_length) {
      bit_writer.write(LZSS_UNENCODED_FLAG, 1);
      bit_writer.write(data.lookahead_buffer_at(0));
      data.slide_window();
    } else {
      bit_writer.write(LZSS_ENCODED_FLAG, 1);
      bit_writer.write(match.position, position_bits);
      bit_writer.write(match.length - minimum_match_length, length_bits);
      data.slide_window(match.length);
    }

    // Keep count of the number of steps for decoding
    ++steps;
  }

  return steps;
}

#endif /* LZSS_ENCODER_H */
