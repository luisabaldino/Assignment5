#include <iostream>
using namespace std;

template <typename T>
class ListNode{
public:
	//Instance variables
	T data;
	ListNode<T> *next;
	ListNode<T> *prev;

	ListNode(); //Constructor
	ListNode(T d); //Overloaded constructor
	~ListNode(); //Destructor
};

template <typename T>
ListNode<T>::ListNode(){
	next = NULL;
	prev = NULL;
}

template <typename T>
ListNode<T>::ListNode(T d){
  data = d;
  next = NULL;
	prev = NULL;
}

template <typename T>
ListNode<T>::~ListNode(){
  next = NULL;
	prev = NULL;
	delete next;
	delete prev;
}
