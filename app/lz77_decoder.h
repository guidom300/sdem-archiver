#ifndef LZ77_DECODER_H
#define LZ77_DECODER_H

#include <deque>
#include <limits>
#include "bit_reader.h"
#include "utils.h"

template <size_t position_bits, size_t length_bits>
struct LZ77MatchRetriever {
  template <typename BitReader, typename MatchType, typename T>
  bool operator()(BitReader& bit_reader, MatchType& match, T& symbol) const {
    bit_reader.read(match.position, position_bits);
    bit_reader.read(match.length, length_bits);
    bit_reader.read(symbol);
    return true;
  }
};

template <bits_t position_bits, bits_t length_bits, typename T = char>
class LZ77Decoder {
 public:
  typedef T symbol_type;

  template <
      typename InputIterator,
      typename OutputIterator,
      typename MatchRetriever = LZ77MatchRetriever<position_bits, length_bits>>
  void operator()(InputIterator begin,
                  InputIterator end,
                  OutputIterator output_iterator,
                  size_t times = std::numeric_limits<size_t>::max(),
                  MatchRetriever match_retriever = MatchRetriever());

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
