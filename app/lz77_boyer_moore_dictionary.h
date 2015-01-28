#ifndef LZ77_BOYER_MOORE_DICTIONARY_H
#define LZ77_BOYER_MOORE_DICTIONARY_H

#include <vector>
#include "utils.h"

/**
 * A class implementing the Boyer-Moore algorithm adapted for LZ77/LZSS.
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
class LZ77BoyerMooreDictionary {
  static constexpr size_t alphabet_size() { return 1 << 8 * sizeof(T); }

 public:
  /**
   * Construct a LZ77BoyerMooreDictionary.
   */
  LZ77BoyerMooreDictionary() {
    _bad_character_table.reserve(alphabet_size());
    _suffix.reserve(max_lookahead_buffer_size);
    _good_suffix_table.reserve(max_lookahead_buffer_size + 1);
  }

  typedef Match<max_dictionary_size, max_lookahead_buffer_size> match_type;

  /**
   * Find a match.
   *
   * @param data the dictionary and lookahead buffer to use
   */
  template <typename Data>
  match_type find_match(const Data& data);

 private:
  typedef typename Integer<max_dictionary_size>::type signed_type;
  typedef std::vector<signed_type> signed_table_type;
  MakeUnsigned<T> make_unsigned;

  signed_table_type _bad_character_table;

  signed_table_type _suffix;
  signed_table_type _good_suffix_table;

  template <typename Data>
  void preprocess_bad_character_rule(const Data& data);

  template <typename Data>
  void preprocess_good_suffix_rule(const Data& data);
};

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
template <typename Data>
typename LZ77BoyerMooreDictionary<max_dictionary_size,
                                  max_lookahead_buffer_size,
                                  T>::match_type
LZ77BoyerMooreDictionary<max_dictionary_size,
                         max_lookahead_buffer_size,
                         T>::find_match(const Data& data) {
  // Preprocessing phase
  preprocess_bad_character_rule(data);
  preprocess_good_suffix_rule(data);

  // Searching phase
  match_type match;
  signed_type pattern_length = data.lookahead_buffer_size();
  signed_type size_diff = data.dictionary_size() - pattern_length;

  for (signed_type i, j = 1 - pattern_length, k; j <= size_diff;) {
    i = pattern_length - 1;
    k = i + j;
    for (; i >= 0 && k >= 0 &&
               data.lookahead_buffer_reverse_at(i) ==
                   data.dictionary_reverse_at(k);
         --i, --k)
      ;

    if (i < 0) {
      // Full match
      match.length = pattern_length;
      match.position = j + match.length;
      return match;

    } else {
      // Partial match
      decltype(match.length) current_length = pattern_length - i - 1;

      if (match.length <= current_length && current_length > 0) {
        match.length = current_length;

        if (k < 0) {
          match.position = match.length;
          match.check_repetition(data.dictionary_end() - match.position,
                                 data.lookahead_buffer_begin(),
                                 data.lookahead_buffer_end());
        } else {
          match.position = k + match.length + 1;
        }
      }

      // Jump ahead
      signed_type bad_character_shift =
          k >= 0
              ? _bad_character_table[make_unsigned(
                    data.dictionary_reverse_at(k))] -
                    pattern_length + 1 + i
              : 1;

      j += std::max(_good_suffix_table[i], bad_character_shift);
    }
  }

  return match;
}

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
template <typename Data>
inline void
LZ77BoyerMooreDictionary<max_dictionary_size,
                         max_lookahead_buffer_size,
                         T>::preprocess_bad_character_rule(const Data& data) {
  signed_type pattern_length = data.lookahead_buffer_size();
  _bad_character_table.assign(alphabet_size(), pattern_length);

  signed_type end = pattern_length - 1;
  auto it = data.lookahead_buffer_rbegin();
  for (signed_type i = 0; i < end; ++i, ++it) {
    _bad_character_table[make_unsigned(*it)] = end - i;
  }
}

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
template <typename Data>
inline void
LZ77BoyerMooreDictionary<max_dictionary_size,
                         max_lookahead_buffer_size,
                         T>::preprocess_good_suffix_rule(const Data& data) {
  signed_type pattern_length = data.lookahead_buffer_size();

  // Fill _suffix
  _suffix.assign(pattern_length, 0);

  _suffix[pattern_length - 1] = pattern_length;

  for (signed_type i = pattern_length - 2, g = pattern_length - 1, f = 0;
       i >= 0;
       --i) {
    if (i > g && _suffix[i + pattern_length - 1 - f] < (i - g)) {
      _suffix[i] = _suffix[i + pattern_length - 1 - f];
    } else {
      if (i < g) {
        g = i;
      }
      f = i;

      while (g >= 0 &&
             data.lookahead_buffer_reverse_at(g) ==
                 data.lookahead_buffer_reverse_at(g + pattern_length - 1 - f)) {
        --g;
      }

      _suffix[i] = f - g;
    }
  }

  // Fill _good_suffix_table
  _good_suffix_table.assign(pattern_length + 1, pattern_length);

  signed_type j = 0;
  for (signed_type i = pattern_length - 1; i >= 0; --i) {
    if (_suffix[i] == i + 1) {
      for (; j < pattern_length - 1 - i; ++j) {
        if (_good_suffix_table[j] == pattern_length) {
          _good_suffix_table[j] = pattern_length - 1 - i;
        }
      }
    }
  }

  if (pattern_length > 1) {
    for (signed_type i = 0; i <= pattern_length - 2; ++i) {
      _good_suffix_table[pattern_length - 1 - _suffix[i]] =
          pattern_length - 1 - i;
    }
  }
}

#endif /* LZ77_BOYER_MOORE_DICTIONARY_H */
