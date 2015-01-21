#ifndef ENCODER_WRAPPER_H
#define ENCODER_WRAPPER_H

#include <iterator>
#include "bit_writer.h"

template <typename Encoder>
struct EncoderWrapper {
  template <typename ForwardIterator, typename OutputIterator>
  void operator()(ForwardIterator begin,
                  ForwardIterator end,
                  OutputIterator output_iterator) {
    Encoder encoder;
    size_t count = encoder(begin, end, output_iterator);
    BitWriter<OutputIterator> bit_writer(output_iterator);
    bit_writer.write(count);
  }
};

#endif /* ENCODER_WRAPPER_H */
