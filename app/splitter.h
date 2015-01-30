#ifndef SPLITTER_H
#define SPLITTER_H

#include <thread>
#include <algorithm>
#include "utils.h"
#include "buffers.h"
#include "pipeline.h"

/**
 * A functor that processes a sequence by splitting it into chunks and assigning
 * each chunk to a separate thread; each thread runs a pipeline with two Worker
 * instances.
 *
 * @tparam W1 the type of the first Worker in the pipeline
 * @tparam W2 the type of the second Worker in the pipeline
 * @tparam T  the type of the symbols in the sequence
 */
template <typename W1, typename W2, typename T = char>
class Splitter {
 public:
  template <typename InputIterator, typename OutputIterator>
  void operator()(InputIterator begin,
                  InputIterator end,
                  OutputIterator output_iterator,
                  size_t max_number_of_threads,
                  size_t chunk_size);

 private:
  template <typename OutputIterator, typename Counter>
  static void pipeline(T* input_buffer,
                       size_t chunk_size,
                       OutputIterator output_iterator,
                       std::thread* previous_thread,
                       Counter* counter,
                       BufferPool<T>* buffer_pool);
};

template <typename W1, typename W2, typename T>
template <typename InputIterator, typename OutputIterator>
void Splitter<W1, W2, T>::operator()(InputIterator begin,
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
        W1::prepare_input_buffer(begin, end, input_buffer, chunk_size);

    counter.wait([&] { return counter.value() < max_number_of_threads; });

    counter.increase();

    thread_ptr new_thread =
        new std::thread(pipeline<OutputIterator, counter_type>,
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

template <typename W1, typename W2, typename T>
template <typename OutputIterator, typename Counter>
void Splitter<W1, W2, T>::pipeline(T* input_buffer,
                                   size_t chunk_size,
                                   OutputIterator output_iterator,
                                   std::thread* previous_thread,
                                   Counter* counter,
                                   BufferPool<T>* buffer_pool) {
  PushBackBuffer<T> buffer(input_buffer, chunk_size);
  PushBackBuffer<T> intermediate_buffer(buffer_pool->get());

  W1 first;
  first(buffer.begin(), buffer.end(), std::back_inserter(intermediate_buffer));

  buffer.clear();

  W2 second;
  second(intermediate_buffer.begin(),
         intermediate_buffer.end(),
         std::back_inserter(buffer));

  if (previous_thread) {
    previous_thread->join();
    delete previous_thread;
  }

  std::copy(buffer.begin(), buffer.end(), output_iterator);

  buffer_pool->release(buffer.begin());
  buffer_pool->release(intermediate_buffer.begin());

  counter->decrease();
  counter->notify();
}

#endif /* SPLITTER_H */
