#ifndef PIPELINE_H
#define PIPELINE_H

#include <thread>
#include <algorithm>
#include "buffers.h"

template <typename T,
          typename OutputIterator,
          typename Counter,
          typename FirstWorker,
          typename SecondWorker>
void pipeline(T* input_buffer,
              size_t chunk_size,
              OutputIterator output_iterator,
              std::thread* previous_thread,
              Counter* counter,
              BufferPool<T>* buffer_pool) {
  PushBackBuffer<T> buffer(input_buffer, chunk_size);
  PushBackBuffer<T> intermediate_buffer(buffer_pool->get());

  FirstWorker first;
  first(buffer.begin(), buffer.end(), std::back_inserter(intermediate_buffer));

  buffer.clear();

  SecondWorker second;
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

#endif /* PIPELINE_H */
