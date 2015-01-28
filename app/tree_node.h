#ifndef TREE_NODE_H
#define TREE_NODE_H

/**
 * A binary tree node.
 *
 * @tparam T the type of the data contained in the node.
 */
template <typename T>
struct TreeNode {
  typedef T data_type;
  typedef TreeNode<T>* ptr_type;

  /**
   * Construct a TreeNode.
   *
   * @param _data the data that the node needs to contain
   */
  explicit TreeNode(const T& _data = T())
      : data(_data), left(nullptr), right(nullptr) {}

  /**
   * @return whether the node is a leaf or not
   */
  bool leaf() const { return left == nullptr && right == nullptr; }

  /**
   * Sort TreeNode objects by their @c data.
   */
  bool operator<(const TreeNode<T>& rhs) const { return data < rhs.data; }

  /**
   * Recursively destroy the children.
   */
  ~TreeNode() {
    if (left) {
      delete left;
    }
    if (right) {
      delete right;
    }
  }

  T data;
  ptr_type left;
  ptr_type right;
};

#endif /* TREE_NODE_H */
