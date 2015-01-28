#ifndef LZ77_DATA_H
#define LZ77_DATA_H

#include <iterator>
#include <deque>

/**
 * A wrapper for the dictionary/lookahead-buffer data bundle that LZ77/LZSS
 * needs, optimized to work from random access iterators.
 *
 * @tparam max_dictionary_size the maximum number of symbols that the dictionary
 *                             may contain
 * @tparam max_lookahead_buffer_size the maximum number of symbols that the
 *                                   lookahead buffer may contain
 * @tparam RandomAccessIterator the type of a random access iterator
 * @tparam T                    the type of the symbols
 */
template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename RandomAccessIterator,
          typename T>
class LZ77Data {
 public:
  typedef RandomAccessIterator iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;

  /**
   * Construct a LZ77Data.
   *
   * @param begin an iterator referring to the beginning of a sequence to encode
   * @param end   an iterator referring to past-the-end of a sequence to encode
   */
  LZ77Data(iterator begin, iterator end)
      : _dictionary_begin(begin),
        _lookahead_buffer_begin(begin),
        _lookahead_buffer_end(begin),
        _end(end) {}

  /**
   * @return an iterator referring to the beginning of the dictionary
   */
  iterator dictionary_begin() const { return _dictionary_begin; }

  /**
   * @return a reverse iterator referring to the beginning of the reversed
   *         dictionary
   */
  reverse_iterator dictionary_rbegin() const {
    return reverse_iterator(dictionary_end());
  }

  /**
   * @return an iterator referring to past-the-end of the dictionary
   */
  iterator dictionary_end() const { return _lookahead_buffer_begin; }

  /**
   * @return a reverse iterator referring to past-the-end of the reversed
   *         dictionary
   */
  reverse_iterator dictionary_rend() const {
    return reverse_iterator(dictionary_begin());
  }

  /**
   * @return an iterator referring to the beginning of the lookahead buffer
   */
  iterator lookahead_buffer_begin() const { return _lookahead_buffer_begin; }

  /**
   * @return a reverse iterator referring to the beginning of the reversed
   *         lookahead buffer
   */
  reverse_iterator lookahead_buffer_rbegin() const {
    return reverse_iterator(lookahead_buffer_end());
  }

  /**
   * @return an iterator referring to past-the-end of the lookahead buffer
   */
  iterator lookahead_buffer_end() const { return _lookahead_buffer_end; }

  /**
   * @return a reverse iterator referring to past-the-end of the reversed
   *         lookahead buffer
   */
  reverse_iterator lookahead_buffer_rend() const {
    return reverse_iterator(lookahead_buffer_begin());
  }

  /**
   * @return the number of symbols in the dictionary
   */
  size_t dictionary_size() const {
    return std::distance(dictionary_begin(), dictionary_end());
  }

  /**
   * @return the number of symbols in the lookahead buffer
   */
  size_t lookahead_buffer_size() const {
    return std::distance(lookahead_buffer_begin(), lookahead_buffer_end());
  }

  /**
   * @return whether the lookahead buffer is empty or not
   */
  bool lookahead_buffer_empty() const { return lookahead_buffer_size() == 0; }

  /**
   * Lookup a symbol in the dictionary.
   *
   * @param i the index
   * @return the requested symbol
   */
  const T& dictionary_at(size_t i) const { return dictionary_begin()[i]; }

  /**
   * Lookup a symbol in the lookahead buffer.
   *
   * @param i the index
   * @return the requested symbol
   */
  const T& lookahead_buffer_at(size_t i) const {
    return lookahead_buffer_begin()[i];
  }

  /**
   * Lookup a symbol in the dictionary, as if it was reversed.
   *
   * @param i the index
   * @return the requested symbol
   */
  const T& dictionary_reverse_at(size_t i) const {
    return dictionary_begin()[dictionary_size() - i - 1];
  }

  /**
   * Lookup a symbol in the lookahead buffer, as if it was reversed.
   *
   * @param i the index
   * @return the requested symbol
   */
  const T& lookahead_buffer_reverse_at(size_t i) const {
    return lookahead_buffer_begin()[lookahead_buffer_size() - i - 1];
  }

  /**
   * Move symbols from the beginning of the lookahead buffer to the
   * end of the dictionary.
   *
   * @param count the number of symbols to move
   */
  void slide_window(size_t count = 1) {
    std::advance(_lookahead_buffer_begin, count);

    if (dictionary_size() > max_dictionary_size) {
      std::advance(_dictionary_begin, dictionary_size() - max_dictionary_size);
    }
  }

  /**
   * @return whether there are more symbols to load in the lookahead buffer or
   *         not
   */
  bool has_more_symbols() const { return lookahead_buffer_end() != _end; }

  /**
   * Forcedly increase the size of the lookahead buffer by one.
   */
  void load_one_more() { ++_lookahead_buffer_end; }

  /**
   * Load as many symbols as possible into the lookahead buffer.
   */
  void fill_lookahead_buffer() {
    while (has_more_symbols() &&
           lookahead_buffer_size() < max_lookahead_buffer_size) {
      load_one_more();
    }
  }

  /**
   * @return whether there are more symbols to encode or not
   */
  bool empty() { return lookahead_buffer_empty() && !has_more_symbols(); }

 private:
  iterator _dictionary_begin;
  iterator _lookahead_buffer_begin;
  iterator _lookahead_buffer_end;
  iterator _end;
};

/**
 * A wrapper for the dictionary/lookahead-buffer data bundle that LZ77/LZSS
 * needs, optimized to work from generic input iterators.
 *
 * @tparam max_dictionary_size the maximum number of symbols that the dictionary
 *                             may contain
 * @tparam max_lookahead_buffer_size the maximum number of symbols that the
 *                                   lookahead buffer may contain
 * @tparam InputIterator the type of an input iterator
 * @tparam T             the type of the symbols
 */
template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename InputIterator,
          typename T>
class LZ77BufferedData {
  typedef std::deque<T> container_type;

 public:
  typedef typename container_type::iterator iterator;
  typedef typename container_type::const_iterator const_iterator;
  typedef typename container_type::const_reverse_iterator
      const_reverse_iterator;

  /**
   * Construct a LZ77BufferedData.
   *
   * @param begin an iterator referring to the beginning of a sequence to
   *              encode
   * @param end   an iterator referring to past-the-end of a sequence to
   *              encode
   */
  LZ77BufferedData(InputIterator begin, InputIterator end)
      : _begin(begin), _end(end) {}

  /**
   * Construct a LZ77BufferedData.
   *
   * @param dictionary_begin an iterator referring to the beginning of a
   *                         dictionary
   * @param dictionary_end   an iterator referring to past-the-end of a
   *                         dictionary
   * @param lookahead_buffer_begin an iterator referring to the beginning of
   *                               a lookahead buffer
   * @param lookahead_buffer_end   an iterator referring to past-the-end of
   *                               a lookahead buffer
   */
  template <typename It1, typename It2>
  LZ77BufferedData(It1 dictionary_begin,
                   It1 dictionary_end,
                   It2 lookahead_buffer_begin,
                   It2 lookahead_buffer_end)
      : _dictionary(dictionary_begin, dictionary_end),
        _lookahead_buffer(lookahead_buffer_begin, lookahead_buffer_end),
        _begin(nullptr),
        _end(nullptr) {}

  /**
   * @return an iterator referring to the beginning of the dictionary
   */
  const_iterator dictionary_begin() const { return _dictionary.cbegin(); }

  /**
   * @return a reverse iterator referring to the beginning of the reversed
   *         dictionary
   */
  const_reverse_iterator dictionary_rbegin() const {
    return _dictionary.crbegin();
  }

  /**
   * @return an iterator referring to past-the-end of the dictionary
   */
  const_iterator dictionary_end() const { return _dictionary.cend(); }

  /**
   * @return a reverse iterator referring to past-the-end of the reversed
   *         dictionary
   */
  const_reverse_iterator dictionary_rend() const { return _dictionary.crend(); }

  /**
   * @return an iterator referring to the beginning of the lookahead buffer
   */
  const_iterator lookahead_buffer_begin() const {
    return _lookahead_buffer.cbegin();
  }

  /**
   * @return a reverse iterator referring to the beginning of the reversed
   *         lookahead buffer
   */
  const_reverse_iterator lookahead_buffer_rbegin() const {
    return _lookahead_buffer.crbegin();
  }

  /**
   * @return an iterator referring to past-the-end of the lookahead buffer
   */
  const_iterator lookahead_buffer_end() const {
    return _lookahead_buffer.cend();
  }

  /**
   * @return a reverse iterator referring to past-the-end of the reversed
   *         lookahead buffer
   */
  const_reverse_iterator lookahead_buffer_rend() const {
    return _lookahead_buffer.crend();
  }

  /**
   * @return the number of symbols in the dictionary
   */
  size_t dictionary_size() const { return _dictionary.size(); }

  /**
   * @return the number of symbols in the lookahead buffer
   */
  size_t lookahead_buffer_size() const { return _lookahead_buffer.size(); }

  /**
   * @return whether the lookahead buffer is empty or not
   */
  bool lookahead_buffer_empty() const { return lookahead_buffer_size() == 0; }

  /**
   * Lookup a symbol in the dictionary.
   *
   * @param i the index
   * @return the requested symbol
   */
  const T& dictionary_at(size_t i) const { return _dictionary.at(i); }

  /**
   * Lookup a symbol in the lookahead buffer.
   *
   * @param i the index
   * @return the requested symbol
   */
  const T& lookahead_buffer_at(size_t i) const {
    return _lookahead_buffer.at(i);
  }

  /**
   * Lookup a symbol in the dictionary, as if it was reversed.
   *
   * @param i the index
   * @return the requested symbol
   */
  const T& dictionary_reverse_at(size_t i) const {
    return _dictionary.at(_dictionary.size() - i - 1);
  }

  /**
   * Lookup a symbol in the lookahead buffer, as if it was reversed.
   *
   * @param i the index
   * @return the requested symbol
   */
  const T& lookahead_buffer_reverse_at(size_t i) const {
    return _lookahead_buffer.at(_lookahead_buffer.size() - i - 1);
  }

  /**
   * Move symbols from the beginning of the lookahead buffer to the
   * end of the dictionary.
   *
   * @param count the number of symbols to move
   */
  void slide_window(size_t count = 1) {
    auto first = _lookahead_buffer.begin();
    auto last = _lookahead_buffer.begin() + count;

    std::copy(first, last, std::back_inserter(_dictionary));

    _lookahead_buffer.erase(first, last);

    if (dictionary_size() > max_dictionary_size) {
      _dictionary.erase(
          _dictionary.begin(),
          _dictionary.begin() + (_dictionary.size() - max_dictionary_size));
    }
  }

  /**
   * @return whether there are more symbols to load in the lookahead buffer or
   *         not
   */
  bool has_more_symbols() const { return _begin != _end; }

  /**
   * Forcedly increase the size of the lookahead buffer by one.
   */
  void load_one_more() {
    _lookahead_buffer.push_back(*_begin);
    ++_begin;
  }

  /**
   * Load as many symbols as possible into the lookahead buffer.
   */
  void fill_lookahead_buffer() {
    while (has_more_symbols() &&
           lookahead_buffer_size() < max_lookahead_buffer_size) {
      load_one_more();
    }
  }

  /**
   * @return whether there are more symbols to encode or not
   */
  bool empty() { return lookahead_buffer_empty() && !has_more_symbols(); }

 private:
  container_type _dictionary;
  container_type _lookahead_buffer;
  InputIterator _begin;
  InputIterator _end;
};

/**
 * A struct with a typedef member @c value that automatically selects the most
 * appropriate data type for LZ77/LZSS encoders.
 *
 * @tparam max_dictionary_size the maximum number of symbols that the dictionary
 *                             may contain
 * @tparam max_lookahead_buffer_size the maximum number of symbols that the
 *                                   lookahead buffer may contain
 * @tparam InputIterator    the type of an input iterator
 * @tparam T                the type of the symbols
 * @tparam IteratorCategory the iterator category
 */
template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename InputIterator,
          typename T,
          typename IteratorCategory =
              typename std::iterator_traits<InputIterator>::iterator_category>
struct DataType {
  typedef LZ77BufferedData<max_dictionary_size,
                           max_lookahead_buffer_size,
                           InputIterator,
                           T> value;
};

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename InputIterator,
          typename T>
struct DataType<max_dictionary_size,
                max_lookahead_buffer_size,
                InputIterator,
                T,
                std::random_access_iterator_tag> {
  typedef LZ77Data<max_dictionary_size,
                   max_lookahead_buffer_size,
                   InputIterator,
                   T> value;
};

#endif /* LZ77_DATA_H */
