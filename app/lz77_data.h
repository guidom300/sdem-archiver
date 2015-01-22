#ifndef LZ77_DATA_H
#define LZ77_DATA_H

#include <iterator>
#include <deque>

template <size_t max_dictionary_size,
          size_t max_lookahead_buffer_size,
          typename RandomAccessIterator,
          typename T>
class LZ77Data {
 public:
  typedef RandomAccessIterator iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;

  LZ77Data(iterator begin, iterator end)
      : _dictionary_begin(begin),
        _lookahead_buffer_begin(begin),
        _lookahead_buffer_end(begin),
        _end(end) {}

  iterator dictionary_begin() const { return _dictionary_begin; }

  reverse_iterator dictionary_rbegin() const {
    return reverse_iterator(dictionary_end());
  }

  iterator dictionary_end() const { return _lookahead_buffer_begin; }

  reverse_iterator dictionary_rend() const {
    return reverse_iterator(dictionary_begin());
  }

  iterator lookahead_buffer_begin() const { return _lookahead_buffer_begin; }

  reverse_iterator lookahead_buffer_rbegin() const {
    return reverse_iterator(lookahead_buffer_end());
  }

  iterator lookahead_buffer_end() const { return _lookahead_buffer_end; }

  reverse_iterator lookahead_buffer_rend() const {
    return reverse_iterator(lookahead_buffer_begin());
  }

  size_t dictionary_size() const {
    return std::distance(dictionary_begin(), dictionary_end());
  }

  size_t lookahead_buffer_size() const {
    return std::distance(lookahead_buffer_begin(), lookahead_buffer_end());
  }

  bool lookahead_buffer_empty() const { return lookahead_buffer_size() == 0; }

  const T& dictionary_at(size_t i) const { return dictionary_begin()[i]; }

  const T& lookahead_buffer_at(size_t i) const {
    return lookahead_buffer_begin()[i];
  }

  const T& dictionary_reverse_at(size_t i) const {
    return dictionary_begin()[dictionary_size() - i - 1];
  }

  const T& lookahead_buffer_reverse_at(size_t i) const {
    return lookahead_buffer_begin()[lookahead_buffer_size() - i - 1];
  }

  void slide_window(size_t count = 1) {
    std::advance(_lookahead_buffer_begin, count);

    if (dictionary_size() > max_dictionary_size) {
      std::advance(_dictionary_begin, dictionary_size() - max_dictionary_size);
    }
  }

  bool has_more_symbols() const { return lookahead_buffer_end() != _end; }

  void load_one_more() { ++_lookahead_buffer_end; }

  void fill_lookahead_buffer() {
    while (has_more_symbols() &&
           lookahead_buffer_size() < max_lookahead_buffer_size) {
      load_one_more();
    }
  }

  bool empty() { return lookahead_buffer_empty() && !has_more_symbols(); }

 private:
  iterator _dictionary_begin;
  iterator _lookahead_buffer_begin;
  iterator _lookahead_buffer_end;
  iterator _end;
};

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

  LZ77BufferedData(InputIterator begin, InputIterator end)
      : _begin(begin), _end(end) {}

  template <typename It1, typename It2>
  LZ77BufferedData(It1 dictionary_begin,
                   It1 dictionary_end,
                   It2 lookahead_buffer_begin,
                   It2 lookahead_buffer_end)
      : _dictionary(dictionary_begin, dictionary_end),
        _lookahead_buffer(lookahead_buffer_begin, lookahead_buffer_end),
        _begin(nullptr),
        _end(nullptr) {}

  const_iterator dictionary_begin() const { return _dictionary.cbegin(); }

  const_reverse_iterator dictionary_rbegin() const {
    return _dictionary.crbegin();
  }

  const_iterator dictionary_end() const { return _dictionary.cend(); }

  const_reverse_iterator dictionary_rend() const { return _dictionary.crend(); }

  const_iterator lookahead_buffer_begin() const {
    return _lookahead_buffer.cbegin();
  }

  const_reverse_iterator lookahead_buffer_rbegin() const {
    return _lookahead_buffer.crbegin();
  }

  const_iterator lookahead_buffer_end() const {
    return _lookahead_buffer.cend();
  }

  const_reverse_iterator lookahead_buffer_rend() const {
    return _lookahead_buffer.crend();
  }

  size_t dictionary_size() const { return _dictionary.size(); }

  size_t lookahead_buffer_size() const { return _lookahead_buffer.size(); }

  bool lookahead_buffer_empty() const { return lookahead_buffer_size() == 0; }

  const T& dictionary_at(size_t i) const { return _dictionary.at(i); }

  const T& lookahead_buffer_at(size_t i) const {
    return _lookahead_buffer.at(i);
  }

  const T& dictionary_reverse_at(size_t i) const {
    return _dictionary.at(_dictionary.size() - i - 1);
  }

  const T& lookahead_buffer_reverse_at(size_t i) const {
    return _lookahead_buffer.at(_lookahead_buffer.size() - i - 1);
  }

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

  bool has_more_symbols() const { return _begin != _end; }

  void load_one_more() {
    _lookahead_buffer.push_back(*_begin);
    ++_begin;
  }

  void fill_lookahead_buffer() {
    while (has_more_symbols() &&
           lookahead_buffer_size() < max_lookahead_buffer_size) {
      load_one_more();
    }
  }

  bool empty() { return lookahead_buffer_empty() && !has_more_symbols(); }

 private:
  container_type _dictionary;
  container_type _lookahead_buffer;
  InputIterator _begin;
  InputIterator _end;
};

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
