#ifndef BUFFERS_H
#define BUFFERS_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include "utils.h"

/**
 * A wrapper for a C-style array that implements a @c push_back() operation.
 *
 * This class is meant to be used with an @c std::back_inserter.
 *
 * @tparam T the type of the values contained in the array
 */
template <typename T>
class PushBackBuffer {
 public:
  typedef T value_type;

  /**
   * Construct a PushBackBuffer.
   *
   * @param[in,out] data the raw buffer
   * @param         size the number of elements already initialized
   */
  PushBackBuffer(T* data, size_t size = 0) : _data(data), _size(size) {}

  /**
   * Append @c value to the end of the buffer.
   *
   * @param[in] value the value of the element to append
   */
  void push_back(const T& value) {
    _data[_size] = value;
    ++_size;
  }

  /**
   * @return an iterator referring to the first element of the buffer
   */
  T* begin() const { return _data; }

  /**
   * @return an iterator referring to the past-the-end element in the buffer
   */
  T* end() const { return _data + _size; }

  void clear() { _size = 0; }

  /**
   * @return the number of elements in the buffer
   */
  size_t size() const { return _size; }

 private:
  T* _data;
  size_t _size;
};

/**
 * A thread-safe pool of C-style arrays.
 *
 * @tparam T the type of the values contained in the arrays
 */
template <typename T>
class BufferPool {
  BufferPool(const BufferPool&) = delete;
  BufferPool& operator=(const BufferPool&) = delete;

 public:
  /**
   * Construct a BufferPool.
   *
   * @param count the number of buffers in the pool
   * @param size  the size of each buffer in the pool
   */
  BufferPool(size_t count, size_t size)
      : _count(count),
        _size(size),
        _data(new T[count * size]),
        _mutexes(new std::mutex[count]),
        _free(count) {}

  ~BufferPool() {
    delete[] _data;
    delete[] _mutexes;
  }

  /**
   * @return a free buffer
   */
  T* get();

  /**
   * Stop using a buffer and return it to the pool.
   *
   * @param buffer the buffer to return to the pool
   */
  void release(T* buffer);

  /**
   * @return the size of each buffer in the pool
   */
  size_t buffer_size() const { return _size; }

 private:
  size_t _count;
  size_t _size;
  T* _data;
  std::mutex* _mutexes;
  ThreadSafeCounter<size_t> _free;
  std::mutex _lock_guard_mutex;
  typedef std::lock_guard<std::mutex> lock_guard_type;
};

template <typename T>
T* BufferPool<T>::get() {
  lock_guard_type lock_guard(_lock_guard_mutex);

  _free.wait([&] { return _free.value() > 0; });

  for (size_t i = 0; i < _count; ++i) {
    if (_mutexes[i].try_lock()) {
      _free.decrease();
      return _data + i * _size;
    }
  }

  return nullptr;
}

template <typename T>
void BufferPool<T>::release(T* buffer) {
  lock_guard_type lock_guard(_lock_guard_mutex);
  _mutexes[(buffer - _data) / _size].unlock();
  _free.increase();
  _free.notify();
}

#endif /* BUFFERS_H */
