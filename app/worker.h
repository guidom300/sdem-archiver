#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include "utils.h"
#include "buffers.h"
#include "pipeline.h"

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

template <typename T,
          typename FirstWorker,
          typename SecondWorker,
          typename InputIterator,
          typename OutputIterator>
void split(InputIterator begin,
           InputIterator end,
           OutputIterator output_iterator,
           size_t max_number_of_threads,
           size_t chunk_size) {
  typedef std::thread* thread_ptr;
  typedef ThreadSafeCounter<decltype(max_number_of_threads)> counter_type;

  thread_ptr previous_thread = nullptr;
  counter_type counter(0);
  BufferPool<char> buffer_pool(2 * max_number_of_threads + 1, chunk_size * 2);

  while (begin != end) {
    auto input_buffer = buffer_pool.get();
    size_t current_chunk_size =
        FirstWorker::prepare_input_buffer(begin, end, input_buffer, chunk_size);

    counter.wait([&] { return counter.value() < max_number_of_threads; });

    counter.increase();

    thread_ptr new_thread = new std::thread(
        pipeline<char, OutputIterator, counter_type, FirstWorker, SecondWorker>,
        input_buffer,
        current_chunk_size,
        output_iterator,
        previous_thread,
        &counter,
        &buffer_pool);

    previous_thread = new_thread;
  }

  previous_thread->join();
  delete previous_thread;
}

#endif /* WORKER_H */
