#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <unordered_map>
#include <queue>
#include <vector>
#include "huffman_tree_base.h"

template <typename T, typename W = size_t>
class HuffmanTree : public HuffmanTreeBase<T, W> {
 public:
  template <typename InputIterator>
  HuffmanTree(InputIterator begin, InputIterator end);

  W frequency(const T& symbol) const {
    return _symbol_frequency_pairs.at(symbol);
  }

 private:
  typedef HuffmanTreeBase<T, W> base_type;
  typedef typename base_type::node_type node_type;
  typedef typename base_type::ptr_type ptr_type;
  typedef typename base_type::data_type data_type;

  struct node_comparator {
    bool operator()(const ptr_type& lhs, const ptr_type& rhs) {
      return !(lhs->operator<(*rhs));
    }
  };

  std::unordered_map<T, W> _symbol_frequency_pairs;
};

template <typename T, typename W>
template <typename InputIterator>
HuffmanTree<T, W>::HuffmanTree(InputIterator begin, InputIterator end) {
  // Count occurrences
  for (; begin != end; ++begin) {
    ++_symbol_frequency_pairs[*begin];
  }

  // Create the tree
  std::priority_queue<ptr_type, std::vector<ptr_type>, node_comparator> queue;

  for (auto&& pair : _symbol_frequency_pairs) {
    queue.push(new node_type(data_type(pair.first, pair.second)));
  }

  while (queue.size() > 1) {
    ptr_type first = queue.top();
    queue.pop();

    ptr_type second = queue.top();
    queue.pop();

    ptr_type new_node = new node_type(first, second);
    new_node->left = first;
    new_node->right = second;

    queue.push(new_node);
  }

  this->_root = queue.top();
}

#endif /* HUFFMAN_TREE_H */
