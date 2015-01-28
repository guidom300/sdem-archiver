#ifndef CANONICAL_HUFFMAN_TREE_H
#define CANONICAL_HUFFMAN_TREE_H

#include <algorithm>
#include <bitset>
#include <unordered_map>
#include <vector>
#include "huffman_tree_base.h"
#include "bit_reader.h"

/**
 * An HuffmanTree built from a canonical codebook.
 *
 * @tparam T the type of the symbols
 * @tparam W the type of the weights
 */
template <typename T, typename W = size_t>
class CanonicalHuffmanTree : public HuffmanTreeBase<T, W> {
 public:
  /**
   * Construct a CanonicalHuffmanTree.
   *
   * @param begin an iterator referring to the beginning of an encoded
   *              sequence of symbols
   * @param end   an iterator referring to past-the-end of an encoded
   *              sequence of symbols
   */
  template <typename InputIterator>
  CanonicalHuffmanTree(InputIterator begin, InputIterator end);

  /**
   * Construct a CanonicalHuffmanTree.
   *
   * @param codebook a non-canonical Huffman codebook
   */
  template <typename HuffmanCodebook>
  CanonicalHuffmanTree(const HuffmanCodebook& codebook);

 private:
  typedef HuffmanTreeBase<T, W> base_type;
  typedef typename base_type::symbol_type symbol_type;
  typedef typename base_type::data_type data_type;
  typedef typename base_type::node_type node_type;
  typedef typename base_type::ptr_type ptr_type;

  typedef unsigned char code_length_type;
  typedef typename std::unordered_map<T, code_length_type> input_codebook_type;

  template <typename InputIterator>
  void initialize(InputIterator begin, InputIterator end);
};

template <typename T, typename W>
template <typename InputIterator>
CanonicalHuffmanTree<T, W>::CanonicalHuffmanTree(InputIterator begin,
                                                 InputIterator end) {
  BitReader<InputIterator> bit_reader(begin, end);
  input_codebook_type input_codebook;

  size_t pairs;
  bit_reader.read(pairs);

  while (pairs--) {
    symbol_type symbol;
    bit_reader.read(symbol);

    code_length_type length;
    bit_reader.read(length);

    if (length > 0) {
      input_codebook[symbol] = length;
    }
  }

  initialize(input_codebook.begin(), input_codebook.end());
}

template <>
template <typename InputIterator>
CanonicalHuffmanTree<char>::CanonicalHuffmanTree(InputIterator begin,
                                                 InputIterator end) {
  BitReader<InputIterator> bit_reader(begin, end);
  input_codebook_type input_codebook;

  for (auto i = 0; i < 256; ++i) {
    code_length_type length;
    bit_reader.read(length);

    if (length > 0) {
      input_codebook[static_cast<char>(i)] = length;
    }
  }

  initialize(input_codebook.begin(), input_codebook.end());
}

template <typename T, typename W>
template <typename HuffmanCodebook>
CanonicalHuffmanTree<T, W>::CanonicalHuffmanTree(
    const HuffmanCodebook& codebook) {
  input_codebook_type input_codebook;

  for (auto&& pair : codebook) {
    code_length_type length = pair.second.size();

    if (length > 0) {
      input_codebook[pair.first] = length;
    }
  }

  initialize(input_codebook.begin(), input_codebook.end());
}

template <typename T, typename W>
template <typename InputIterator>
void CanonicalHuffmanTree<T, W>::initialize(InputIterator begin,
                                            InputIterator end) {
  // Sort the (non-canonical) codebook
  std::vector<data_type> symbol_length_pairs;
  for (; begin != end; ++begin) {
    symbol_length_pairs.emplace_back(begin->first, begin->second);
  }

  std::sort(symbol_length_pairs.begin(), symbol_length_pairs.end());

  // Calculate a canonical Huffman code book
  typedef std::bitset<32> bitset_type;
  bitset_type code;
  std::unordered_map<T, bitset_type> canonical_codebook;
  size_t current_length = 0;

  for (auto&& data : symbol_length_pairs) {
    code <<= data.length - current_length;
    canonical_codebook[data.symbol] = code;
    code = code.to_ulong() + 1;
    current_length = data.length;
  }

  // Translate the codebook to a tree
  this->_root = new node_type();
  for (auto&& data : symbol_length_pairs) {
    code = canonical_codebook[data.symbol];

    ptr_type node = this->_root;

    for (size_t i = 1; i <= data.length; ++i) {
      if (code[data.length - i]) {
        if (!node->right) {
          node->right = new node_type();
        }
        node = node->right;
      } else {
        if (!node->left) {
          node->left = new node_type();
        }
        node = node->left;
      }
    }

    node->data.symbol = data.symbol;
  }
}

#endif /* CANONICAL_HUFFMAN_TREE_H */
