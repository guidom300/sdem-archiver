#ifndef HUFFMAN_ENCODER_STACK_H
#define HUFFMAN_ENCODER_STACK_H

#include <iterator>
#include "huffman_tree.h"
#include "huffman_encoder.h"

template <typename T>
struct HuffmanEncoderStack {
  template <typename ForwardIterator, typename OutputIterator>
  void operator()(ForwardIterator begin,
                  ForwardIterator end,
                  OutputIterator output_iterator) {
    HuffmanTree<T> tree(begin, end);
    HuffmanEncoder<T> encoder(tree);
    encoder.make_canonical();
    encoder.dump_codebook(output_iterator);
    encoder(begin, end, output_iterator);
  }

  template <typename InputStream, typename OutputStream>
  void operator()(InputStream& input_stream, OutputStream& output_stream) {
    typedef std::istreambuf_iterator<T> input_iterator_t;
    typedef std::ostreambuf_iterator<T> output_iterator_t;

    HuffmanTree<T> tree(input_iterator_t(input_stream.rdbuf()),
                        input_iterator_t());

    HuffmanEncoder<T> encoder(tree);
    encoder.make_canonical();

    input_stream.seekg(0);

    output_iterator_t output_iterator(output_stream);
    encoder.dump_codebook(output_iterator);

    encoder(input_iterator_t(input_stream.rdbuf()),
            input_iterator_t(),
            output_iterator);
  }
};

#endif /* HUFFMAN_ENCODER_STACK_H */
