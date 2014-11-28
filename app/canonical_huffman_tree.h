#ifndef CANONICAL_HUFFMAN_TREE_H
#define CANONICAL_HUFFMAN_TREE_H

#include <algorithm>
#include <bitset>
#include <unordered_map>
#include <vector>
#include "huffman_tree_base.h"

template <typename T, typename W = size_t>
class CanonicalHuffmanTree : public HuffmanTreeBase<T, W> {
 public:
  template <typename InputIterator>
  CanonicalHuffmanTree(InputIterator begin, InputIterator end);

 private:
  typedef HuffmanTreeBase<T, W> base_type;
  typedef typename base_type::data_type data_type;
  typedef typename base_type::node_type node_type;
  typedef typename base_type::ptr_type ptr_type;
};

template <typename T, typename W>
template <typename InputIterator>
CanonicalHuffmanTree<T, W>::CanonicalHuffmanTree(InputIterator begin,
                                                 InputIterator end) {
  // Sort the (non-canonical) codebook
  std::vector<data_type> symbol_length_pairs;
  for (; begin != end; ++begin) {
    symbol_length_pairs.emplace_back(begin->first, begin->second.size());
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
