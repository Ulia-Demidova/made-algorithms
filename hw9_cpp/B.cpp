#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::pair;
using std::string;
using std::vector;

struct Node {
  int value;
  int priority;
  int size;
  Node *left;
  Node *right;
  explicit Node(int value, int y)
      : value(value), priority(y), size(1), left(nullptr), right(nullptr) {}
};

class Treap {
private:
  Node *root = nullptr;

  int get_size(Node *&vertex) {
    if (vertex)
      return vertex->size;
    else
      return 0;
  }

  void fix(Node *&vertex) {
    vertex->size = get_size(vertex->left) + get_size(vertex->right) + 1;
  }

  pair<Node *, Node *> split(Node *&vertex, int idx) {
    if (!vertex)
      return {nullptr, nullptr};

    if (get_size(vertex->left) > idx) {
      pair<Node *, Node *> tree_split = split(vertex->left, idx);
      vertex->left = tree_split.second;
      fix(vertex);
      return {tree_split.first, vertex};
    } else {
      pair<Node *, Node *> tree_split =
          split(vertex->right, idx - get_size(vertex->left) - 1);
      vertex->right = tree_split.first;
      fix(vertex);
      return {vertex, tree_split.second};
    }
  }

  Node *&merge(Node *&root1, Node *&root2) {
    if (!root1)
      return root2;
    if (!root2)
      return root1;
    if (root1->priority > root2->priority) {
      root1->right = merge(root1->right, root2);
      fix(root1);
      return root1;
    } else {
      root2->left = merge(root1, root2->left);
      fix(root2);
      return root2;
    }
  }

  void del_(Node *&vertex, int idx) {
    if (!vertex)
      return;

    if (get_size(vertex->left) == idx) {
      Node *temp = vertex;
      vertex = merge(vertex->left, vertex->right);
      delete temp;
    } else if (get_size(vertex->left) > idx) {
      del_(vertex->left, idx);
      fix(vertex);
    } else {
      del_(vertex->right, idx - get_size(vertex->left) - 1);
      fix(vertex);
    }
  }

public:
  void insert(int idx, int value, int priority) {
    pair<Node *, Node *> tree_split = split(root, idx);
    Node *new_vertex = new Node(value, priority);
    root = merge(merge(tree_split.first, new_vertex), tree_split.second);
  }

  void del(int idx) { del_(root, idx - 1); }

  void fill_vector(vector<int> &vec, Node *&vertex, size_t start = 0) {
    if (!vertex)
      return;
    vec[get_size(vertex->left) + start] = vertex->value;
    fill_vector(vec, vertex->left, start);
    fill_vector(vec, vertex->right, start + get_size(vertex->left) + 1);
  }

  void print_array() {
    if (!root)
      cout << 0 << '\n';
    else {
      cout << root->size << '\n';
      vector<int> vec(root->size);
      fill_vector(vec, root);
      for (auto val : vec)
        cout << val << ' ';
    }
  }
};

int main() {
  int n, m;
  cin >> n >> m;
  Treap tree;
  int value;
  for (size_t i = 0; i < n; ++i) {
    cin >> value;
    tree.insert(i - 1, value, rand());
  }
  string command;
  int idx;
  for (size_t i = 0; i < m; ++i) {
    cin >> command >> idx;
    if (command == "add") {
      cin >> value;
      tree.insert(idx - 1, value, rand());
    } else
      tree.del(idx);
  }
  tree.print_array();
  return 0;
}
