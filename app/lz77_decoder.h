#ifndef LZ77_DECODER_H
#define LZ77_DECODER_H

#include <deque>
#include <limits>
#include "bit_reader.h"
#include "utils.h"

/**
 * A functor for retrieving Match instances from a LZ77-encoded sequence.
 *
 * @tparam position_bits the number of bits used to encode the match position
 * @tparam length_bits   the number of bits used to encode the match length
 */
template <size_t position_bits, size_t length_bits>
struct LZ77MatchRetriever {
  /**
   * @param[out] bit_reader the BitReader to use
   * @param[out] match      the Match to initialize
   * @param[out] symbol     the symbol to initialize
   * @return true
   */
  template <typename BitReader, typename MatchType, typename T>
  bool operator()(BitReader& bit_reader, MatchType& match, T& symbol) const {
    bit_reader.read(match.position, position_bits);
    bit_reader.read(match.length, length_bits);
    bit_reader.read(symbol);
    return true;
  }
};

/**
 * A functor that decodes a sequence of symbols encoded with LZ77.
 *
 * @tparam position_bits the number of bits used to encode the match position
 * @tparam length_bits   the number of bits used to encode the match length
 * @tparam T             the type of the symbols
 */
template <bits_t position_bits, bits_t length_bits, typename T = char>
class LZ77Decoder {
 public:
  typedef T symbol_type;

  /**
   * Decode a sequence of symbols.
   *
   * @param begin an input iterator referring to the beginning of the encoded
   *              sequence
   * @param end   an input iterator referring to past-the-end of the encoded
   *              sequence
   * @param output_iterator an output iterator for writing the decoded sequence
   * @param times           the number of matches to process
   * @param match_retriever the functor to use for retrieving single matches
   */
  template <
      typename InputIterator,
      typename OutputIterator,
      typename MatchRetriever = LZ77MatchRetriever<position_bits, length_bits>>
  void operator()(InputIterator begin,
                  InputIterator end,
                  OutputIterator output_iterator,
                  size_t times = std::numeric_limits<size_t>::max(),
                  MatchRetriever match_retriever = MatchRetriever());

  /**
   * Remove all symbols from the internal dictionary.
   */
  void clear() { _dictionary.clear(); }

 private:
  typedef std::deque<T> dictionary_type;
  dictionary_type _dictionary;

  static constexpr size_t max_dictionary_size = max_size(position_bits);
  typedef Match<max_dictionary_size, max_size(length_bits)> match_type;

  template <typename OutputIterator>
  void write_symbol(OutputIterator& output_iterator, symbol_type symbol);

  void resize_dictionary();
};

template <bits_t position_bits, bits_t length_bits, typename T>
template <typename InputIterator,
          typename OutputIterator,
          typename MatchRetriever>
inline void LZ77Decoder<position_bits, length_bits, T>::operator()(
    InputIterator begin,
    InputIterator end,
    OutputIterator output_iterator,
    size_t times,
    MatchRetriever match_retriever) {
  BitReader<InputIterator> bit_reader(begin, end);

  for (size_t steps = 0; steps < times && bit_reader; ++steps) {
    match_type match;
    symbol_type symbol;

    bool append_symbol = match_retriever(bit_reader, match, symbol);

    for (size_t i = _dictionary.size() - match.position, j = 0;
         j < match.length;
         ++j) {
      write_symbol(output_iterator, _dictionary[i + (j % match.position)]);
    }

    if (append_symbol) {
      write_symbol(output_iterator, symbol);
    }

    resize_dictionary();
  }
}

template <bits_t position_bits, bits_t length_bits, typename T>
template <typename OutputIterator>
inline void LZ77Decoder<position_bits, length_bits, T>::write_symbol(
    OutputIterator& output_iterator, symbol_type symbol) {
  *output_iterator = symbol;
  _dictionary.push_back(symbol);
}

template <bits_t position_bits, bits_t length_bits, typename T>
inline void LZ77Decoder<position_bits, length_bits, T>::resize_dictionary() {
  if (_dictionary.size() > max_dictionary_size) {
    _dictionary.erase(
        _dictionary.begin(),
        _dictionary.begin() + (_dictionary.size() - max_dictionary_size));
  }
}

#endif /* LZ77_DECODER_H */
