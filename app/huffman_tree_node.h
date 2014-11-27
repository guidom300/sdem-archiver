#ifndef HUFFMAN_TREE_NODE_H
#define HUFFMAN_TREE_NODE_H

#include "tree_node.h"

template <typename T, typename W>
struct HuffmanData {
  typedef T symbol_type;
  typedef W weight_type;

  explicit HuffmanData(const T& _symbol = T(), const W& _weight = W())
      : symbol(_symbol), weight(_weight) {}

  explicit HuffmanData(const W& _weight) : symbol(), weight(_weight) {}

  bool operator<(const HuffmanData& rhs) const { return weight < rhs.weight; }

  T symbol;

  union {
    W weight;
    W length;
  };
};

template <typename T, typename W>
struct HuffmanTreeNode : TreeNode<HuffmanData<T, W>> {
  typedef TreeNode<HuffmanData<T, W>> base_type;
  typedef typename base_type::ptr_type ptr_type;
  typedef typename base_type::data_type data_type;

  explicit HuffmanTreeNode(const data_type& data) : base_type(data) {}

  explicit HuffmanTreeNode(ptr_type left, ptr_type right)
      : base_type(data_type(left->data.weight + right->data.weight)) {}
};

#endif /* HUFFMAN_TREE_NODE_H */
