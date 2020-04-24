#pragma once
#include <iostream>
using namespace std;
#include "ListNode.h"
#include <stdexcept>

template <typename T>

//Header File
class GenLinkedList{
public:
		//Instance variables
		ListNode<T> *front;
		ListNode<T> *back;
		unsigned int size;

		GenLinkedList(); //Constructor
		~GenLinkedList(); //Destructor

		void insertFront(T data); //Insert node w/ data to the front of list
		void insertBack(T data); //Insert node w/ data to the back of list
		T* removeFront(); //Remove first element of list
		T getElement(int idx); //Returns data at a certan index
		T remove(T val); //Removes node w/ specific data

		unsigned int getSize(); //Returns size of list
		bool isEmpty(); //Returns whether list is empty
		void printList(); //Prints every element of list
};
template <class T>
GenLinkedList<T>::GenLinkedList(){
 size = 0;
 front = new ListNode<T>();
 back = new ListNode<T>();
}
template <class T>
GenLinkedList<T>::~GenLinkedList(){
 delete size;
 front = NULL;
 back = NULL;
 delete front;
 delete back;
}
template <class T>
T GenLinkedList<T>::getElement(int idx){
	ListNode<T> *curr = front;
 	int counter = 0;
	//Loop through list and return value if it matches given index
	while (counter != idx){
		curr = curr->next;
		counter ++;
	}
	return curr->data;
}
template <class T>
unsigned int GenLinkedList<T>::getSize(){
 return size;
}
template <class T>
bool GenLinkedList<T>::isEmpty(){
 return (size == 0);
}
template <class T>
void GenLinkedList<T>::printList(){
 ListNode<T> *curr = front;

 while (curr != NULL){
	 cout << curr->data << endl;
	 curr = curr->next;
 }
}
template <class T>
void GenLinkedList<T>::insertFront(T data){
 ListNode<T> *node = new ListNode<T>(data);

 if(isEmpty()){
	 back = node;
 } else {
	 //not empty
	 front->prev = node;
	 node->next = front;
 }
 front = node;
 size++;
}
template <class T>
void GenLinkedList<T>::insertBack(T data){
 ListNode<T> *node = new ListNode<T>(data);

 if(isEmpty()){
	 front = node;
 } else {
	 //not empty
	 back->next = node;
	 node->prev = back;
	 node->next = NULL;
 }
 back = node;
 size++;
}

template <class T>
T* GenLinkedList<T>::removeFront(){
 ListNode<T> *tempNode = front;
 //error check, make sure list is not empty before attempting to remove
 if (size == 0){
	 throw std::out_of_range( "Tried to remove from empty queue." );
	}
 	else {
	 if (front->next == NULL){
		 //there is only one node in the list
		 back = NULL;
	 } else{
		 //more than one node in the list
		 front->next->prev = NULL;
	 }

	 front = front->next;
	 tempNode->next = NULL;
	 T* temp = tempNode->data;
	 size--;
	 delete tempNode;

	 return temp;
	}
}
template <class T>
T GenLinkedList<T>::remove(T value){
 ListNode<T> *curr = front;
 //Lets look for this value
 while(curr->data != value){
	 curr = curr->next;
 }
 //we found the value, let's delete it
 if(curr == front){
	 //if node to be deleted is the front
	 front = curr->next;
 } else{
	 //node to be deleted is somewhere between front and back
	 curr->prev->next = curr->next;
 }
 if(curr == back){
	 //node to be deleted is at the back
	 back = curr->prev;
 } else{
	 //node to be deleted is in between
	 curr->next->prev = curr->prev;
 }
 curr->next = NULL;
 curr ->prev = NULL;
 T temp = curr->data;
 size--;
 delete curr;

 return temp;
}
