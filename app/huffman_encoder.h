#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include <vector>
#include <unordered_map>
#include "bit_writer.h"
#include "huffman_tree.h"
#include "canonical_huffman_tree.h"

/**
 * A functor that encodes a sequence of symbols using Huffman coding.
 *
 * @tparam T the type of symbols
 * @tparam W the type of the weights
 */
template <typename T, typename W = size_t>
class HuffmanEncoder {
 public:
  typedef T symbol_type;
  typedef W length_type;

  typedef std::vector<bool> bitset_type;
  typedef std::unordered_map<T, bitset_type> codebook_type;

  /**
   * Construct an HuffmanEncoder.
   *
   * @param[in] huffman_tree an HuffmanTree to use to build the codebook
   */
  template <typename HuffmanTree>
  explicit HuffmanEncoder(const HuffmanTree& huffman_tree) {
    navigate(huffman_tree.root());
  }

  /**
   * Construct an HuffmanEncoder.
   *
   * @param first an input iterator referring to the beginning of a
   *              non-canonical Huffman codebook
   * @param last  an input iterator referring to past-the-end of a non-canonical
   *              Huffman codebook
   */
  template <typename InputIterator>
  HuffmanEncoder(InputIterator first, InputIterator last)
      : _codebook(first, last) {}

  /**
   * Encode a sequence of symbols.
   *
   * @param begin an input iterator referring to the beginning of the sequence
   *              to encode
   * @param end   an input iterator referring to past-the-end of the sequence to
   *              encode
   * @param output_iterator an output iterator for writing the encoded sequence
   * @return a count to be used for EOF/EOS management
   */
  template <typename InputIterator, typename OutputIterator>
  size_t operator()(InputIterator begin,
                    InputIterator end,
                    OutputIterator output_iterator) const;

  /**
   * Lookup a symbol in the codebook.
   *
   * @param symbol the symbol to lookup
   * @return the code associated with the specified symbol
   */
  const bitset_type& operator[](symbol_type symbol) const {
    return _codebook.at(symbol);
  }

  /**
   * Make the encoding canonical.
   */
  void make_canonical();

  /**
   * Write the number of encoded bits to an output iterator.
   *
   * @param[in]  tree            an HuffmanTree
   * @param      output_iterator an output iterator
   */
  template <typename OutputIterator>
  void dump_encoded_bits(const HuffmanTree<T, W>& tree,
                         OutputIterator output_iterator) const;

  /**
   * Write the codebook to an output iterator.
   *
   * @param output_iterator an output iterator
   */
  template <typename OutputIterator>
  void dump_codebook(OutputIterator output_iterator) const;

  /**
   * Write the codebook and the number of encoded bits to an output iterator.
   *
   * @param[in]  tree            an HuffmanTree
   * @param      output_iterator an output iterator
   */
  template <typename OutputIterator>
  void dump_header(const HuffmanTree<T, W>& tree,
                   OutputIterator output_iterator) const {
    dump_codebook(output_iterator);
    dump_encoded_bits(tree, output_iterator);
  }

 private:
  codebook_type _codebook;

  template <typename TreeNode>
  void navigate(const TreeNode* node, const bitset_type& code = bitset_type());

  typedef unsigned char code_length_type;
};

template <typename T, typename W>
template <typename InputIterator, typename OutputIterator>
size_t HuffmanEncoder<T, W>::operator()(InputIterator begin,
                                        InputIterator end,
                                        OutputIterator output_iterator) const {
  size_t count = 0;
  BitWriter<OutputIterator> bit_writer(output_iterator);

  for (; begin != end; ++begin, ++count) {
    bit_writer.write_bitset(_codebook.at(*begin));
  }

  return count;
}

template <typename T, typename W>
template <typename TreeNode>
void HuffmanEncoder<T, W>::navigate(const TreeNode* node,
                                    const bitset_type& code) {
  if (node->leaf()) {
    _codebook[node->data.symbol] = code;
  } else {
    auto child = node->left;
    if (child) {
      bitset_type new_code(code);
      new_code.push_back(false);
      navigate(child, new_code);
    }

    child = node->right;
    if (child) {
      bitset_type new_code(code);
      new_code.push_back(true);
      navigate(child, new_code);
    }
  }
}

template <typename T, typename W>
void HuffmanEncoder<T, W>::make_canonical() {
  CanonicalHuffmanTree<T, W> canonical_huffman_tree(_codebook);
  _codebook.clear();
  navigate(canonical_huffman_tree.root());
}

template <typename T, typename W>
template <typename OutputIterator>
void HuffmanEncoder<T, W>::dump_codebook(OutputIterator output_iterator) const {
  BitWriter<OutputIterator> bit_writer(output_iterator);

  bit_writer.write(_codebook.size());

  for (auto&& codebook_pair : _codebook) {
    bit_writer.write(codebook_pair.first);
    bit_writer.write(
        static_cast<code_length_type>(codebook_pair.second.size()));
  }
}

template <>
template <typename OutputIterator>
void HuffmanEncoder<char>::dump_codebook(OutputIterator output_iterator) const {
  BitWriter<OutputIterator> bit_writer(output_iterator);

  for (auto i = 0; i < 256; ++i) {
    char ch = i;
    auto it = _codebook.find(ch);
    code_length_type length;

    if (it != _codebook.end()) {
      length = it->second.size();
    } else {
      length = 0;
    }

    bit_writer.write(length);
  }
}

template <typename T, typename W>
template <typename OutputIterator>
void HuffmanEncoder<T, W>::dump_encoded_bits(
    const HuffmanTree<T, W>& tree, OutputIterator output_iterator) const {
  size_t encoded_bits = 0;

  for (auto&& pair : _codebook) {
    encoded_bits += pair.second.size() * tree.frequency(pair.first);
  }

  BitWriter<OutputIterator> bit_writer(output_iterator);
  bit_writer.write(encoded_bits);
}

#endif /* HUFFMAN_ENCODER_H */
