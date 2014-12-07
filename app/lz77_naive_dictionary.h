#ifndef LZ77_NAIVE_DICTIONARY_H
#define LZ77_NAIVE_DICTIONARY_H

#include <cstddef>
#include <deque>
#include "utils.h"

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
class LZ77NaiveDictionary {
 public:
  LZ77NaiveDictionary() = default;

  template <typename InputIterator>
  LZ77NaiveDictionary(InputIterator first, InputIterator last) {
    slide_window(first, last);
  }

  typedef match_type<max_dictionary_size, max_lookahead_buffer_size> match_type;

  template <typename BidirectionalIterator>
  match_type find_match(BidirectionalIterator begin,
                        BidirectionalIterator end) const;

  void clear();

  template <typename InputIterator>
  void slide_window(InputIterator begin, InputIterator end);

 private:
  std::deque<T> _dictionary;
};

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
template <typename BidirectionalIterator>
typename LZ77NaiveDictionary<max_dictionary_size,
                             max_lookahead_buffer_size,
                             T>::match_type
LZ77NaiveDictionary<max_dictionary_size,
                    max_lookahead_buffer_size,
                    T>::find_match(BidirectionalIterator begin,
                                   BidirectionalIterator end) const {
  // Find a match (without repetition)
  auto dictionary_current = _dictionary.cbegin();
  auto dictionary_end = _dictionary.cend();

  auto pos = dictionary_current;
  match_type match;

  while (dictionary_current != dictionary_end) {
    auto it1 = dictionary_current;
    auto it2 = begin;
    decltype(match.length) current_length = 0;

    while (it1 != dictionary_end && it2 != end && *it1 == *it2) {
      ++current_length;
      ++it1;
      ++it2;
    }

    if (current_length >= match.length) {
      pos = dictionary_current;
      match.length = current_length;
    }

    ++dictionary_current;
  }

  match.position = dictionary_end - pos;

  match.check_repetition(_dictionary.cend() - match.position, begin, end);

  return match;
}

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
inline void LZ77NaiveDictionary<max_dictionary_size,
                                max_lookahead_buffer_size,
                                T>::clear() {
  _dictionary.clear();
}

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename T>
template <typename InputIterator>
inline void LZ77NaiveDictionary<max_dictionary_size,
                                max_lookahead_buffer_size,
                                T>::slide_window(InputIterator begin,
                                                 InputIterator end) {
  _dictionary.insert(_dictionary.end(), begin, end);

  auto size = _dictionary.size();
  if (size > max_dictionary_size) {
    auto dbegin = _dictionary.begin();
    _dictionary.erase(dbegin, dbegin + (size - max_dictionary_size));
  }
}

#endif /* LZ77_NAIVE_DICTIONARY_H */
