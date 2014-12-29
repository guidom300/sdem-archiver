#ifndef LZ77_BOYER_MOORE_DICTIONARY_H
#define LZ77_BOYER_MOORE_DICTIONARY_H

#include <algorithm>
#include <deque>
#include <iterator>
#include <vector>
#include "utils.h"

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
class LZ77BoyerMooreDictionary {
  static constexpr size_t alphabet_size() { return 1 << 8 * sizeof(T); }

 public:
  LZ77BoyerMooreDictionary() : _bad_character_table(alphabet_size()) {}

  template <typename InputIterator>
  LZ77BoyerMooreDictionary(InputIterator first, InputIterator last)
      : LZ77BoyerMooreDictionary() {
    slide_window(first, last);
  }

  typedef Match<max_dictionary_size, max_lookahead_buffer_size> match_type;

  template <typename BidirectionalIterator>
  match_type find_match(BidirectionalIterator begin, BidirectionalIterator end);

  void clear();

  template <typename InputIterator>
  void slide_window(InputIterator begin, InputIterator end);

 private:
  std::deque<T> _dictionary;

  typedef typename Integer<max_lookahead_buffer_size>::type signed_type;
  typedef std::vector<signed_type> signed_table_type;
  signed_table_type _bad_character_table;

  typedef typename UnsignedInteger<max_lookahead_buffer_size>::type
      unsigned_type;
  typedef std::vector<unsigned_type> unsigned_table_type;
  unsigned_table_type _suffix;
  unsigned_table_type _good_suffix_table;

  template <typename InputIterator>
  void preprocess_bad_character_rule(InputIterator begin,
                                     size_t pattern_length);

  template <typename BidirectionalIterator>
  void preprocess_good_suffix_rule(BidirectionalIterator begin,
                                   size_t pattern_length);

  template <typename InputIterator, typename Distance>
  auto at(InputIterator it, Distance distance) -> decltype(*it) {
    std::advance(it, distance);
    return *it;
  }
};

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
template <typename BidirectionalIterator>
typename LZ77BoyerMooreDictionary<max_dictionary_size,
                                  max_lookahead_buffer_size,
                                  T>::match_type
LZ77BoyerMooreDictionary<max_dictionary_size,
                         max_lookahead_buffer_size,
                         T>::find_match(BidirectionalIterator begin,
                                        BidirectionalIterator end) {
  // Reverse the (perceived) order of the lookahead buffer
  typedef std::reverse_iterator<BidirectionalIterator> pattern_iterator_type;
  pattern_iterator_type first(end);
  pattern_iterator_type last(begin);

  // Preprocessing phase
  auto pattern_length = std::distance(begin, end);

  preprocess_bad_character_rule(first, pattern_length);
  preprocess_good_suffix_rule(first, pattern_length);

  // Searching phase
  match_type match;
  signed_type size_diff = _dictionary.size() - pattern_length;

  for (signed_type i, j = 1 - pattern_length, k; j <= size_diff;) {
    i = pattern_length - 1;
    k = i + j;
    for (; i >= 0 && k >= 0 && at(first, i) == _dictionary[k]; --i, --k)
      ;

    if (i < 0) {
      // Full match
      match.length = pattern_length;
      match.position = j + match.length;
      return match;

    } else {
      // Partial match
      decltype(match.length) current_length = pattern_length - i - 1;

      if (match.length <= current_length) {
        match.length = current_length;

        if (k < 0) {
          match.position = match.length;
        } else {
          match.position = j + match.length;
        }
      }

      // Jump ahead
      auto bad_character_shift =
          _bad_character_table[_dictionary[i + j]] - pattern_length + 1 + i;

      j += std::max(
          static_cast<decltype(bad_character_shift)>(_good_suffix_table[i]),
          bad_character_shift);
    }
  }

  match.check_repetition(_dictionary.rend() - match.position, begin, end);

  return match;
}

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
template <typename InputIterator>
inline void
LZ77BoyerMooreDictionary<max_dictionary_size, max_lookahead_buffer_size, T>::
    preprocess_bad_character_rule(InputIterator begin, size_t pattern_length) {
  _bad_character_table.assign(_bad_character_table.size(),
                              static_cast<signed_type>(pattern_length));

  for (unsigned_type i = 0; i < pattern_length - 1; ++i, ++begin) {
    _bad_character_table[*begin] = pattern_length - i - 1;
  }
}

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
template <typename BidirectionalIterator>
inline void LZ77BoyerMooreDictionary<
    max_dictionary_size,
    max_lookahead_buffer_size,
    T>::preprocess_good_suffix_rule(BidirectionalIterator begin,
                                    size_t pattern_length) {
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

      while (g >= 0 && at(begin, g) == at(begin, g + pattern_length - 1 - f)) {
        --g;
      }

      _suffix[i] = f - g;
    }
  }

  // Fill _good_suffix_table
  _good_suffix_table.assign(pattern_length + 1, pattern_length);

  unsigned_type j = 0;
  for (signed_type i = pattern_length - 1; i >= 0; --i) {
    if (_suffix[i] == i + 1) {
      for (; j < pattern_length - 1 - i; ++j) {
        if (static_cast<decltype(pattern_length)>(_good_suffix_table[j]) ==
            pattern_length) {
          _good_suffix_table[j] =
              static_cast<unsigned_type>(pattern_length - 1 - i);
        }
      }
    }
  }

  for (unsigned_type i = 0; i <= pattern_length - 2; ++i) {
    _good_suffix_table[pattern_length - 1 - _suffix[i]] =
        static_cast<unsigned_type>(pattern_length - 1 - i);
  }
}

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
template <typename InputIterator>
inline void LZ77BoyerMooreDictionary<max_dictionary_size,
                                     max_lookahead_buffer_size,
                                     T>::slide_window(InputIterator begin,
                                                      InputIterator end) {
  std::copy(begin, end, std::front_inserter(_dictionary));

  if (_dictionary.size() > max_dictionary_size) {
    auto dend = _dictionary.end();
    _dictionary.erase(dend - (_dictionary.size() - max_dictionary_size), dend);
  }
}

#endif /* LZ77_BOYER_MOORE_DICTIONARY_H */
