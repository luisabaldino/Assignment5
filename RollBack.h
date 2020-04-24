#include <iostream>
#include "GenStack.h"
#include "Student.h"
#include "Faculty.h"
#include "GenBST.h"
using namespace std;

class RollBack{
public:

  GenBST<Student> *studentTree;
  GenBST<Faculty> *facultyTree;

  struct RBtuple {
    int group;
    string transactionType;
    string treeType;
    Student stud;
    Faculty facu;
  };

  GenStack<RBtuple> *transactionStack = new GenStack<RBtuple>;
  RBtuple tuple;

  RollBack(GenBST<Student> *sTree, GenBST<Faculty> *fTree);
  ~RollBack();

  void store(int group, string transactionType, string treeType, Student stud);
  void store(int group, string transactionType, string treeType, Faculty facu);
  int getLastGroup();
  void undo(int numItems);
  void undoTuple(RBtuple tuple);

};
RollBack::RollBack(GenBST<Student> *sTree, GenBST<Faculty> *fTree){
  studentTree = sTree;
  facultyTree = fTree;
}
RollBack::~RollBack(){
}

void RollBack::store(int group, string transactionType, string treeType, Student stud){
    tuple.group = group;
    tuple.transactionType = transactionType;
    tuple.treeType = treeType;
    tuple.stud = stud;
    transactionStack->push(tuple);
}
void RollBack::store(int group, string transactionType, string treeType, Faculty facu){
    tuple.group = group;
    tuple.transactionType = transactionType;
    tuple.treeType = treeType;
    tuple.facu = facu;
    transactionStack->push(tuple);
}

int RollBack::getLastGroup() {
  // Returns the largest group number used in the transactionStack
  // this will always be the group from the tuple at the top of the stack
  int lastgroup = 0;
  try {
    tuple = transactionStack->peek();
    lastgroup = tuple.group;
  } catch (...) {
  }
  return lastgroup;
}

void RollBack::undo(int steps){
  // Each "step" the user wants to undo corresponds to a group of transactions
  cout << "Rolling back up to " << steps << " steps..." << endl;
  if (transactionStack->isEmpty()) {
    cout << "There are no transactions to roll back." << endl;
  } else {
    // There are transactions to roll back...
    int currgroup = getLastGroup();
    int stepcnt = 0;
    while (stepcnt < steps) {
      try {
          // popping from the stack can throw exceptions... >:(
          tuple = transactionStack->pop();
          if (currgroup != tuple.group) {
            // Group number changed, increment step
            stepcnt++;
            currgroup = tuple.group;
          }
          if (stepcnt < steps) {
             // We have the data to process the undo
             undoTuple(tuple);
          }
      } catch (...) {
        // pop will throw an exeption when trying to pop from empty stack
        break;
      }
    }
  }
  cout << "Rollback complete." << endl;
}

// Undoes a record insert, delete, or update
void RollBack::undoTuple(RBtuple tuple) {
  cout << "Rolling back " << tuple.treeType << " " << tuple.transactionType << "..." << endl;
  if (tuple.treeType == "Student") {
    if (studentTree->search(tuple.stud)) studentTree->deleteNode(tuple.stud);
    if (tuple.transactionType != "insert") studentTree->insert(tuple.stud);
  } else if (tuple.treeType == "Faculty") {
    if (facultyTree->search(tuple.facu)) facultyTree->deleteNode(tuple.facu);
    if (tuple.transactionType != "insert") facultyTree->insert(tuple.facu);
  }
}
