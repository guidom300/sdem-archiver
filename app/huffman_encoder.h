#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include <vector>
#include <unordered_map>
#include "huffman_tree.h"
#include "bit_writer.h"

template <typename T, typename W = size_t>
class HuffmanEncoder {
 public:
  typedef T symbol_type;
  typedef std::vector<bool> bitset_type;
  typedef std::unordered_map<T, bitset_type> codebook_type;

  template <typename HuffmanTree>
  explicit HuffmanEncoder(const HuffmanTree& huffman_tree) {
    navigate(huffman_tree.root());
  }

  template <typename InputIterator, typename OutputIterator>
  size_t operator()(InputIterator begin,
                    InputIterator end,
                    OutputIterator output_iterator);

  const bitset_type& operator[](symbol_type symbol) const {
    return _codebook.at(symbol);
  }

 private:
  codebook_type _codebook;

  template <typename TreeNode>
  void navigate(const TreeNode* node, const bitset_type& key = bitset_type());
};

template <typename T, typename W>
template <typename InputIterator, typename OutputIterator>
size_t HuffmanEncoder<T, W>::operator()(InputIterator begin,
                                        InputIterator end,
                                        OutputIterator output_iterator) {
  size_t count = 0;
  BitWriter<OutputIterator> bit_writer(output_iterator);

  for (; begin != end; ++begin, ++count) {
    bit_writer.write_bitset(_codebook[*begin]);
  }

  return count;
}

template <typename T, typename W>
template <typename TreeNode>
void HuffmanEncoder<T, W>::navigate(const TreeNode* node,
                                    const bitset_type& key) {
  if (node->leaf()) {
    _codebook[node->data.symbol] = key;
  } else {
    auto child = node->left;
    if (child) {
      bitset_type new_key(key);
      new_key.push_back(false);
      navigate(child, new_key);
    }

    child = node->right;
    if (child) {
      bitset_type new_key(key);
      new_key.push_back(true);

      navigate(child, new_key);
    }
  }
}

#endif /* HUFFMAN_ENCODER_H */
