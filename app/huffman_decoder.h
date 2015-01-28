#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include "huffman_tree_base.h"
#include "bit_reader.h"
#include "bit_writer.h"

/**
 * A functor that decodes a sequence of symbols encoded using Huffman coding.
 *
 * @tparam T the type of symbols
 * @tparam W the type of the weights
 */
template <typename T, typename W = size_t>
class HuffmanDecoder {
 public:
  typedef T symbol_type;

  /**
   * Construct an HuffmanDecoder.
   *
   * @param huffman_tree an HuffmanTree to use for decoding the symbols
   */
  template <typename HuffmanTree>
  explicit HuffmanDecoder(const HuffmanTree& huffman_tree)
      : root(huffman_tree.root()) {}

  /**
   * Decode a sequence of symbols.
   *
   * @param begin an input iterator referring to the beginning of the encoded
   *              sequence
   * @param end   an input iterator referring to past-the-end of the encoded
   *              sequence
   * @param output_iterator an output iterator for writing the decoded sequence
   * @param skip_header whether or not @c begin points to the beginning of the
   *                    header
   */
  template <typename InputIterator, typename OutputIterator>
  void operator()(InputIterator begin,
                  InputIterator end,
                  OutputIterator output_iterator,
                  bool skip_header = false);

 private:
  typedef typename HuffmanTreeBase<T, W>::ptr_type ptr_type;
  ptr_type root;

  size_t header_size() const;
};

template <typename T, typename W>
template <typename InputIterator, typename OutputIterator>
void HuffmanDecoder<T, W>::operator()(InputIterator begin,
                                      InputIterator end,
                                      OutputIterator output_iterator,
                                      bool skip_header) {
  if (skip_header) {
    std::advance(begin, header_size());
  }

  BitReader<InputIterator> bit_reader(begin, end);
  BitWriter<OutputIterator> bit_writer(output_iterator);
  ptr_type current_node = root;

  size_t bits_to_read;
  bit_reader.read(bits_to_read);

  while (bit_reader && bits_to_read--) {
    bool bit = bit_reader.read();

    if (bit) {
      current_node = current_node->right;
    } else {
      current_node = current_node->left;
    }

    if (current_node->leaf()) {
      bit_writer.write(current_node->data.symbol);
      current_node = root;
    }
  }
}

template <>
size_t HuffmanDecoder<char>::header_size() const {
  return 256;
}

#endif /* HUFFMAN_DECODER_H */
