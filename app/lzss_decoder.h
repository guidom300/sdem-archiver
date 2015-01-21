#ifndef LZSS_DECODER_H
#define LZSS_DECODER_H

#include "lzss_encoder.h"
#include "lz77_decoder.h"

template <size_t position_bits, size_t length_bits, size_t minimum_match_length>
struct LZSSMatchRetriever {
  template <typename BitReader, typename MatchType, typename T>
  bool operator()(BitReader& bit_reader, MatchType& match, T& symbol) const {
    bool encoded_flag = bit_reader.read();

    if (encoded_flag == LZSS_ENCODED_FLAG) {
      bit_reader.read(match.position, position_bits);
      bit_reader.read(match.length, length_bits);
      match.length += minimum_match_length;
      return false;
    } else {
      bit_reader.read(symbol);
      return true;
    }
  }
};

template <bits_t position_bits,
          bits_t length_bits,
          size_t minimum_match_length =
              get_minimum_match_length(position_bits, length_bits),
          typename T = char>
class LZSSDecoder : public LZ77Decoder<position_bits, length_bits, T> {
 private:
  typedef LZ77Decoder<position_bits, length_bits, T> base_type;

 public:
  template <typename InputIterator, typename OutputIterator>
  void operator()(InputIterator begin,
                  InputIterator end,
                  OutputIterator output_iterator,
                  size_t times = std::numeric_limits<size_t>::max()) {
    base_type::operator()(
        begin,
        end,
        output_iterator,
        times,
        LZSSMatchRetriever<position_bits, length_bits, minimum_match_length>());
  }
};

#endif /* LZSS_DECODER_H */
