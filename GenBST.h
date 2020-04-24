#pragma once
#include <iostream>
#include <fstream>
#include "TreeNode.h"
using namespace std;

template <typename T>

class GenBST{
public:
  GenBST();
  ~GenBST();
  //virtual ~GenBST()

  void insert (T value);
  bool personSearch(int id);
  bool search(T value);
  T retrieve(int id);
  bool deleteNode(T k);

  bool isEmpty();
  TreeNode<T> *getMin();
  TreeNode<T> *getMax();

  TreeNode<T>* getSuccessor(TreeNode<T> *d);
  void printTree(TreeNode<T> *d);
  void printPersonTree(TreeNode<T> *d);

  void saveTree(string fileName);
  void saveAllNodes(ofstream& outFile, TreeNode<T> *node);

//private:
  TreeNode<T> *root;
};
template <class T>
GenBST<T>::GenBST(){
  root = NULL; //empty tree
}
template <class T>
GenBST<T>::~GenBST(){
  //diy
}
template <class T>
void GenBST<T>::printTree(TreeNode<T> *node){
  if(node == NULL) return;

  printTree(node->left);
  cout << node->key << endl;
  printTree(node->right);
}
template <class T>
void GenBST<T>::printPersonTree(TreeNode<T> *node){
  if(node == NULL) return;

  printPersonTree(node->left);
  node->key.displayData();
  printPersonTree(node->right);
}
template <class T>
TreeNode<T>* GenBST<T>::getMax(){
  TreeNode<T> *curr = root;

  if(root == NULL) return NULL;

  while (curr->right != NULL){
    curr = curr->right;
  }
  return curr;
}
template <class T>
TreeNode<T>* GenBST<T>::getMin(){
  TreeNode<T> *curr = root;

  if(root == NULL) return NULL;

  while (curr->left != NULL){
    curr = curr->left;
  }
  return curr;
}
template <class T>
bool GenBST<T>::isEmpty(){
  return (root == NULL);
}
template <class T>
void GenBST<T>::insert(T value){
  TreeNode<T> *node = new TreeNode<T>(value);

  if (isEmpty()){
    //empty tree
    root = node;
  } else{
    //not empty
    TreeNode<T> *curr = root;
    TreeNode<T> *parent;

    while (true){
      parent = curr;
      if(value < curr->key){
        curr = curr->left;
        if(curr == NULL){
          //leaf node = insertion point
          parent->left = node;
          break;
        }
      } else{
        curr = curr->right;
        if(curr == NULL){
          //insertion point
          parent->right = node;
          break;
        }
      }
    }
  }
}
template <class T>
bool GenBST<T>::search(T value){
  if(isEmpty()) return false;
  else {
    //not an empty Tree
    TreeNode<T> *curr = root;
    while(curr->key != value){
      if(value < curr->key)
        curr = curr->left;
      else
        curr = curr->right;

      if(curr == NULL) return false;
    }
  }
  return true;
}

template <class T>
bool GenBST<T>::personSearch(int id){
  if(isEmpty()) return false;
  else {
    //not an empty Tree
    TreeNode<T> *curr = root;
    while(curr->key.id != id){
      if(id < curr->key.id)
        curr = curr->left;
      else
        curr = curr->right;

      if(curr == NULL) return false;
    }
  }
  return true;
}

template <class T>
T GenBST<T>::retrieve(int id){
    TreeNode<T> *curr = root;
    while(curr->key.id != id){
      if(id < curr->key.id)
        curr = curr->left;
      else
        curr = curr->right;
    }
    return curr->key;
  }

template <class T>
bool GenBST<T>::deleteNode(T k){
  if (isEmpty()) return false;

  TreeNode<T> *parent = root;
  TreeNode<T> *curr = root;
  bool isLeft = true;

  //Find node
  while(curr->key != k){
    parent = curr;
    if (k < curr->key){
      isLeft = true;
      curr = curr->left;
    } else{
      isLeft = false;
      curr = curr->right;
    }
    if (curr == NULL) return false;
  }
  //Found value, proceed to delete
  if (curr->left == NULL && curr->right == NULL){
    //Then we have a leaf Treenode
    if (curr == root){
       root = NULL;
     }
    else if (isLeft) parent->left = NULL;
    else parent->right = NULL;
  }

  else if (curr->right == NULL){
    //No right child, must have left child
    if (curr == root) {
      //Root with left child
      root = curr->left;
    } else if (isLeft){
      //Left node with left child
      parent->left = curr->left;
    } else {
      //Right node with left child
      parent->right = curr->left;
    }
  }

  else if (curr->left == NULL){
    //No left child, must have right child
    if (curr == root) {
      //Root with right child
      root = curr->right;
    } else if (isLeft){
      //Left node with right child
      parent->left = curr->right;
    } else {
      //Right node with right child
      parent->right = curr->right;
    }
  }

  else{
    //Two children
    TreeNode<T> *successor = getSuccessor(curr);

    if(curr==root) root = successor;
    else if (isLeft) parent->left = successor;
    else parent->right = successor;

    //connect/link successor to curr's (d) left children
    successor->left = curr->left;

  }

}
template <class T>
TreeNode<T>* GenBST<T>::getSuccessor(TreeNode<T> *d){
  //d represents the node to be deleted

  TreeNode<T> *curr = d->right;
  TreeNode<T> *sp = d; //successor's parent
  TreeNode<T> *successor = d;

  while (curr!=NULL){
    sp = successor;
    successor = curr;
    curr = curr->left;
  }

  if(successor != d->right){
    sp->left = successor->right; //Point to null
    successor->right = d->right;
  }
  return successor;
}

template <class T>
void GenBST<T>::saveTree(string fileName){
  //saveTree presumes all node objects have a serialize function
  ofstream outFile;
  outFile.open(fileName + ".txt", std::ofstream::out | std::ofstream::trunc);
  if (!outFile){
    cout << "Was not able to open file. Please try again." << endl;
  } else {
    saveAllNodes(outFile, root);
  }
  outFile.close();
}

template <class T>
void GenBST<T>::saveAllNodes(ofstream& outFile, TreeNode<T> *node) {
  //saveAllNodes presumes all node objects have a serialize function
  if(node != NULL) {
    outFile << node->key.serialize();
    saveAllNodes(outFile, node->left);
    saveAllNodes(outFile, node->right);
  }
}
