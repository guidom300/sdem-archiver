#ifndef HUFFMAN_TREE_BASE_H
#define HUFFMAN_TREE_BASE_H

#include "huffman_tree_node.h"

/**
 * A tree having HuffmanTreeNode instances as nodes.
 *
 * @tparam T the type of the symbols
 * @tparam W the type of the weights
 */
template <typename T, typename W>
class HuffmanTreeBase {
 public:
  typedef T symbol_type;
  typedef W weight_type;
  typedef HuffmanTreeNode<T, W> node_type;
  typedef typename node_type::ptr_type ptr_type;
  typedef typename node_type::data_type data_type;

  /**
   * Recursively destroy all nodes.
   */
  ~HuffmanTreeBase() { delete _root; }

  /**
   * @return the root of the tree
   */
  const ptr_type root() const { return _root; }

 protected:
  ptr_type _root;
};

#endif /* HUFFMAN_TREE_BASE_H */
