#ifndef HUFFMAN_TREE_NODE_H
#define HUFFMAN_TREE_NODE_H

#include "tree_node.h"

/**
 * A symbol-weight pair.
 *
 * @tparam T the type of the symbol
 * @tparam W the type of the weight
 */
template <typename T, typename W>
struct HuffmanData {
  typedef T symbol_type;
  typedef W weight_type;

  /**
   * Construct an HuffmanData.
   *
   * @param _symbol the symbol
   * @param _weight the weight
   */
  explicit HuffmanData(const T& _symbol = T(), const W& _weight = W())
      : symbol(_symbol), weight(_weight) {}

  /**
   * Construct an HuffmanData.
   *
   * @param _weight the weight
   */
  explicit HuffmanData(const W& _weight) : symbol(), weight(_weight) {}

  /**
   * Sort HuffmanData objects first by @c weight and then by @c symbol.
   */
  bool operator<(const HuffmanData& rhs) const {
    if (weight == rhs.weight) {
      return symbol < rhs.symbol;
    }
    return weight < rhs.weight;
  }

  T symbol;

  union {
    W weight;
    W length;
  };
};

/**
 * A TreeNode containing an HuffmanData instance.
 */
template <typename T, typename W>
struct HuffmanTreeNode : TreeNode<HuffmanData<T, W>> {
  typedef TreeNode<HuffmanData<T, W>> base_type;
  typedef typename base_type::ptr_type ptr_type;
  typedef typename base_type::data_type data_type;

  /**
   * Construct an HuffmanTreeNode.
   *
   * @param data the data that the node needs to contain
   */
  explicit HuffmanTreeNode(const data_type& data = data_type())
      : base_type(data) {}

  /**
   * Construct an HuffmanTreeNode.
   *
   * @param left  a pointer to an HuffmanTreeNode
   * @param right a pointer to an HuffmanTreeNode
   */
  explicit HuffmanTreeNode(ptr_type left, ptr_type right)
      : base_type(data_type(left->data.weight + right->data.weight)) {}
};

#endif /* HUFFMAN_TREE_NODE_H */
