#ifndef BUFFERS_H
#define BUFFERS_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include "utils.h"

template <typename T>
class PushBackBuffer {
 public:
  typedef T value_type;

  PushBackBuffer(T* data, size_t size = 0) : _data(data), _size(size) {}

  void push_back(const T& value) {
    _data[_size] = value;
    ++_size;
  }

  T* begin() const { return _data; }

  T* end() const { return _data + _size; }

  void clear() { _size = 0; }

  size_t size() const { return _size; }

 private:
  T* _data;
  size_t _size;
};

template <typename T>
class BufferPool {
  BufferPool(const BufferPool&) = delete;
  BufferPool& operator=(const BufferPool&) = delete;

 public:
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

  T* get();

  void release(T* buffer);

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
