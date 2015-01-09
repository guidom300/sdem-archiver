#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include <vector>
#include <unordered_map>
#include "bit_writer.h"
#include "huffman_tree.h"
#include "canonical_huffman_tree.h"

template <typename T, typename W = size_t>
class HuffmanEncoder {
 public:
  typedef T symbol_type;
  typedef W length_type;

  typedef std::vector<bool> bitset_type;
  typedef std::unordered_map<T, bitset_type> codebook_type;

  template <typename HuffmanTree>
  explicit HuffmanEncoder(const HuffmanTree& huffman_tree) {
    navigate(huffman_tree.root());
  }

  template <typename InputIterator>
  HuffmanEncoder(InputIterator first, InputIterator last)
      : _codebook(first, last) {}

  template <typename InputIterator, typename OutputIterator>
  size_t operator()(InputIterator begin,
                    InputIterator end,
                    OutputIterator output_iterator) const;

  const bitset_type& operator[](symbol_type symbol) const {
    return _codebook.at(symbol);
  }

  void make_canonical();

  template <typename OutputIterator>
  void dump_encoded_bits(const HuffmanTree<T, W>& tree,
                         OutputIterator output_iterator) const;

  template <typename OutputIterator>
  void dump_codebook(OutputIterator output_iterator) const;

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
