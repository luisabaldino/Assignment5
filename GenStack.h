#include <iostream>
#include <stdexcept>

using namespace std;

template <class T>

class GenStack
{
  public:
    GenStack(); //Default constructor
    GenStack(int maxSize); //Overloaded constructor
    ~GenStack(); //Destructor (add tilde)

    //core functions
    void push(T data); //insert an item
    T pop(); //remove

    //aux/helper functions
    T peek(); //AKA top
    bool isEmpty();
    bool isFull();

  private:
    int top; //index of top element
    int mSize;

    T *myArray; //represent the memory address of the first block
};

//default constructor
template <class T>
GenStack<T>::GenStack(){
  myArray = new T[100];
  mSize = 100;
  top = -1;
}
//overloaded constructor
template <class T>
GenStack<T>::GenStack(int maxSize){
  myArray = new T[maxSize];
  mSize = maxSize;
  top = -1;
}

template <class T>
GenStack<T>::~GenStack(){
  delete myArray;
}

template <class T>
void GenStack<T>::push(T data){
  //check if full
  if (top == mSize) {
    mSize ++;
    T *newArray = new T[mSize];
    for (int i = 0; i < mSize; i++){
      newArray[i] = myArray[i];
    }
    myArray = newArray;
  }
  myArray[++top] = data;
}

template <class T>
T GenStack<T>::pop(){
  //check if emtpy
  if (top == -1) {
    //throw exception
    throw std::out_of_range( "Tried to pop from empty stack." );
  }
  return myArray[top--];
}

template <class T>
T GenStack<T>::peek(){
  //check if emtpy
  if (top == -1) {
    //throw exception
    throw std::out_of_range( "Tried to pop from empty stack." );
  }
  return myArray[top];
}

template <class T>
bool GenStack<T>::isFull(){
  return (top == mSize-1);
}

template <class T>
bool GenStack<T>::isEmpty(){
  return (top == -1);
}
