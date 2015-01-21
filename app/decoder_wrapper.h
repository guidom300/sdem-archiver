#ifndef DECODER_WRAPPER_H
#define DECODER_WRAPPER_H

#include <iterator>
#include "bit_reader.h"

template <typename Decoder>
struct DecoderWrapper {
  template <typename BidirectionalIterator, typename OutputIterator>
  void operator()(BidirectionalIterator begin,
                  BidirectionalIterator end,
                  OutputIterator output_iterator) {
    size_t count;
    std::advance(end, -sizeof(count));
    BitReader<BidirectionalIterator> bit_reader(end);
    bit_reader.read(count);
    std::advance(end, sizeof(count));

    Decoder decoder;
    decoder(begin, end, output_iterator, count);
  }

  template <typename InputStream, typename OutputStream>
  void operator()(InputStream& input_stream, OutputStream& output_stream) {
    size_t count;
    input_stream.seekg(-sizeof(count), std::ios_base::end);
    BitReader<> bit_reader(input_stream);
    bit_reader.read(count);

    input_stream.seekg(0);

    Decoder decoder;
    decoder(std::istreambuf_iterator<char>(input_stream),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(output_stream),
            count);
  }
};

#endif /* DECODER_WRAPPER_H */
