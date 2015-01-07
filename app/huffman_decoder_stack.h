#ifndef HUFFMAN_DECODER_STACK_H
#define HUFFMAN_DECODER_STACK_H

#include "canonical_huffman_tree.h"
#include "huffman_decoder.h"

template <typename T>
struct HuffmanDecoderStack {
  template <typename InputIterator, typename OutputIterator>
  void operator()(InputIterator begin,
                  InputIterator end,
                  OutputIterator output_iterator) {
    CanonicalHuffmanTree<T> tree(begin, end);
    HuffmanDecoder<T> decoder(tree);
    decoder(begin, end, output_iterator);
  }
};

#endif /* HUFFMAN_DECODER_STACK_H */
