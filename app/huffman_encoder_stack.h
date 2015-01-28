#ifndef HUFFMAN_ENCODER_STACK_H
#define HUFFMAN_ENCODER_STACK_H

#include <iterator>
#include "huffman_tree.h"
#include "huffman_encoder.h"

/**
 * A Worker for an HuffmanEncoder.
 *
 * @tparam T the type of the symbols
 */
template <typename T>
struct HuffmanEncoderStack {
  /**
   * Encode a sequence of symbols.
   *
   * @param begin a forward iterator referring to the beginning of the sequence
   *              to encode
   * @param end   a forward iterator referring to past-the-end of the sequence
   *              to encode
   * @param output_iterator an output iterator for writing the encoded sequence
   */
  template <typename ForwardIterator, typename OutputIterator>
  void operator()(ForwardIterator begin,
                  ForwardIterator end,
                  OutputIterator output_iterator) {
    HuffmanTree<T> tree(begin, end);
    HuffmanEncoder<T> encoder(tree);
    encoder.make_canonical();

    encoder.dump_header(tree, output_iterator);

    encoder(begin, end, output_iterator);
  }

  /**
   * Encode a sequence of symbols.
   *
   * @param[out] input_stream  an input stream containing the sequence to encode
   * @param[out] output_stream an output stream for writing the encoded sequence
   */
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
    encoder.dump_header(tree, output_iterator);

    encoder(input_iterator_t(input_stream.rdbuf()),
            input_iterator_t(),
            output_iterator);
  }
};

#endif /* HUFFMAN_ENCODER_STACK_H */
