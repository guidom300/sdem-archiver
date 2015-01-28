#ifndef ENCODER_WRAPPER_H
#define ENCODER_WRAPPER_H

#include <iterator>
#include "bit_writer.h"

/**
 * A wrapper for an encoder Worker that handles EOF/EOS management.
 *
 * @tparam Encoder a Worker that works as an encoder
 */
template <typename Encoder>
struct EncoderWrapper {
  /**
   * Encode a sequence of symbols.
   *
   * @param begin a forward iterator referring to the beginning of the
   *              sequence to encode
   * @param end   a forward iterator referring to past-the-end of the sequence
   *              to encode
   * @param output_iterator an output iterator for writing the encoded sequence
   */
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
