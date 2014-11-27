#ifndef HUFFMAN_TREE_BASE_H
#define HUFFMAN_TREE_BASE_H

#include "huffman_tree_node.h"

template <typename T, typename W>
class HuffmanTreeBase {
 public:
  typedef T symbol_type;
  typedef W weight_type;
  typedef HuffmanTreeNode<T, W> node_type;
  typedef typename node_type::ptr_type ptr_type;
  typedef typename node_type::data_type data_type;

  ~HuffmanTreeBase() { delete _root; }

  const ptr_type root() const { return _root; }

 protected:
  ptr_type _root;
};

#endif /* HUFFMAN_TREE_BASE_H */
