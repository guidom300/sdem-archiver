#ifndef WORKER_H
#define WORKER_H

#include <cstddef>

/**
 * A concept for a functor that has a default constructor, an
 * @c operator()(InputIterator, InputIterator, OutputIterator) and a
 * @c prepare_input_buffer(InputIterator&, InputIterator, PtrType, size_t)
 * member functions.
 *
 * @tpara T the type of the wrapped encoder/decoder
 */
template <typename T>
struct Worker {
  template <typename InputIterator, typename OutputIterator>
  void operator()(InputIterator begin,
                  InputIterator end,
                  OutputIterator output_iterator) {
    T worker;
    worker(begin, end, output_iterator);
  }

  template <typename InputIterator, typename PtrType>
  static size_t prepare_input_buffer(InputIterator& begin,
                                     InputIterator end,
                                     PtrType input_buffer,
                                     size_t chunk_size) {
    size_t current_chunk_size = 0;

    for (; current_chunk_size < chunk_size && begin != end;
         ++current_chunk_size, ++begin) {
      input_buffer[current_chunk_size] = *begin;
    }

    return current_chunk_size;
  }
};

#endif /* WORKER_H */
