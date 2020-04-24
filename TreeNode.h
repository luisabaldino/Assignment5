#include <iostream>
using namespace std;

template <typename T>
class TreeNode{
public:
  TreeNode();
  TreeNode(T k);
  ~TreeNode();

  T key;
  TreeNode<T> *left;
  TreeNode<T> *right;
};
template <typename T>
TreeNode<T>::TreeNode(){
  left = NULL;
  right = NULL;
}
template <typename T>
TreeNode<T>::TreeNode(T k){
  key = k;
  left = NULL;
  right = NULL;
}
template <typename T>
TreeNode<T>::~TreeNode(){
}
