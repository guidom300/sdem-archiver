#ifndef TREE_NODE_H
#define TREE_NODE_H

template <typename T>
struct TreeNode {
  typedef T data_type;
  typedef TreeNode<T>* ptr_type;

  explicit TreeNode(const T& _data = T())
      : data(_data), left(nullptr), right(nullptr) {}

  bool leaf() const { return left == nullptr && right == nullptr; }

  bool operator<(const TreeNode<T>& rhs) const { return data < rhs.data; }

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
