#ifndef LZ77_ENCODER_H
#define LZ77_ENCODER_H

#include "lz77_data.h"
#include "bit_writer.h"
#include "lz77_boyer_moore_dictionary.h"
#include "lz77_naive_dictionary.h"

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
  typedef D<max_dictionary_size, max_lookahead_buffer_size, T> dictionary_type;

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
          size_t max_dictionary_size = max_size(position_bits),
          size_t max_lookahead_buffer_size = max_size(length_bits),
          typename T = char>
using NaiveLZ77Encoder = LZ77Encoder<position_bits,
                                     length_bits,
                                     max_dictionary_size,
                                     max_lookahead_buffer_size,
                                     T,
                                     LZ77NaiveDictionary>;

template <bits_t position_bits,
          bits_t length_bits,
          size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T,
          template <size_t, size_t, typename> class D>
template <typename Data, typename OutputIterator>
size_t LZ77Encoder<position_bits,
                   length_bits,
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

    // Get the next character
    if (match.length == data.lookahead_buffer_size()) {
      if (data.has_more_symbols()) {
        data.load_one_more();
      } else {
        --match.length;
      }
    }

    symbol_type next_symbol = data.lookahead_buffer_at(match.length);

    // Write output
    bit_writer.write(match.position, position_bits);
    bit_writer.write(match.length, length_bits);
    bit_writer.write(next_symbol);

    // Move window
    data.slide_window(match.length + 1);

    // Keep count of the number of steps for decoding
    ++steps;
  }

  return steps;
}

#endif /* LZ77_ENCODER_H */
