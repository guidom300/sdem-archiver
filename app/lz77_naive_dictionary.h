#ifndef LZ77_NAIVE_DICTIONARY_H
#define LZ77_NAIVE_DICTIONARY_H

#include <cstddef>
#include <deque>

template <size_t max_size, typename T>
class LZ77NaiveDictionary {
 public:
  LZ77NaiveDictionary() = default;

  template <typename InputIterator>
  LZ77NaiveDictionary(InputIterator first, InputIterator last)
      : _dictionary(first, last) {}

  struct match_type {
    size_t position;
    size_t length = 0;
  };

  template <typename ForwardIterator>
  match_type find_match(ForwardIterator begin, ForwardIterator end) const;

  void clear();

  template <typename InputIterator>
  void slide_window(InputIterator begin, InputIterator end);

 private:
  std::deque<T> _dictionary;
};

template <size_t max_size, typename T>
template <typename ForwardIterator>
typename LZ77NaiveDictionary<max_size, T>::match_type
LZ77NaiveDictionary<max_size, T>::find_match(ForwardIterator begin,
                                             ForwardIterator end) const {
  // Find a match (without repetition)
  auto first1 = _dictionary.cbegin();
  auto last1 = _dictionary.cend();
  auto first2 = begin;
  auto last2 = end;

  auto pos = first1;
  match_type match;

  while (first1 != last1) {
    auto it1 = first1;
    auto it2 = first2;
    decltype(match.length) current_length = 0;

    while (it1 != last1 && it2 != last2 && *it1 == *it2) {
      ++current_length;
      ++it1;
      ++it2;
    }

    if (current_length >= match.length) {
      pos = first1;
      match.length = current_length;
    }

    ++first1;
  }

  match.position = last1 - pos;

  // Check for repetition
  if (match.length > 0 && match.position == match.length) {
    first1 = _dictionary.cend() - match.position;
    first2 += match.length;
    last2 = end;

    size_t count = 0;
    while (first2 != last2 &&
           *(first1 + (count % match.position)) == *(first2)) {
      ++count;
      ++first2;
    }

    match.length += count;
  }

  return match;
}

template <size_t max_size, typename T>
inline void LZ77NaiveDictionary<max_size, T>::clear() {
  _dictionary.clear();
}

template <size_t max_size, typename T>
template <typename InputIterator>
inline void LZ77NaiveDictionary<max_size, T>::slide_window(InputIterator begin,
                                                           InputIterator end) {
  _dictionary.insert(_dictionary.end(), begin, end);

  auto size = _dictionary.size();
  if (size > max_size) {
    auto dbegin = _dictionary.begin();
    _dictionary.erase(dbegin, dbegin + (size - max_size));
  }
}

#endif /* LZ77_NAIVE_DICTIONARY_H */
