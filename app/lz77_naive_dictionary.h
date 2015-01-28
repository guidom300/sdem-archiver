#ifndef LZ77_NAIVE_DICTIONARY_H
#define LZ77_NAIVE_DICTIONARY_H

#include "utils.h"

/**
 * A class implementing a brute force string matching algorithm for LZ77/LZSS.
 *
 * @tparam max_dictionary_size the maximum number of symbols that the dictionary
 *                             may contain
 * @tparam max_lookahead_buffer_size the maximum number of symbols that the
 *                                   lookahead buffer may contain
 * @tparam T the type of the symbols
 */
template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
class LZ77NaiveDictionary {
 public:
  typedef Match<max_dictionary_size, max_lookahead_buffer_size> match_type;

  /**
   * Find a match.
   *
   * @param data the dictionary and lookahead buffer to use
   */
  template <typename Data>
  match_type find_match(const Data& data) const;
};

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
template <typename Data>
typename LZ77NaiveDictionary<max_dictionary_size,
                             max_lookahead_buffer_size,
                             T>::match_type
LZ77NaiveDictionary<max_dictionary_size,
                    max_lookahead_buffer_size,
                    T>::find_match(const Data& data) const {
  // Find a match (without repetition)
  auto dictionary_current = data.dictionary_begin();
  auto dictionary_end = data.dictionary_end();

  size_t current_position = data.dictionary_size();
  match_type match;

  for (; dictionary_current != dictionary_end;
       ++dictionary_current, --current_position) {
    auto it1 = dictionary_current;
    auto it2 = data.lookahead_buffer_begin();
    decltype(match.length) current_length = 0;

    while (it1 != dictionary_end && it2 != data.lookahead_buffer_end() &&
           *it1 == *it2) {
      ++current_length;
      ++it1;
      ++it2;
    }

    if (current_length >= match.length) {
      match.position = current_position;
      match.length = current_length;
    }
  }

  match.check_repetition(data.dictionary_end() - match.position,
                         data.lookahead_buffer_begin(),
                         data.lookahead_buffer_end());

  if (match.length == 0) {
    match.position = 0;
  }

  return match;
}

#endif /* LZ77_NAIVE_DICTIONARY_H */
