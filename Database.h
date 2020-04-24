#include <iostream>
#include <sstream>
#include "GenBST.h"
#include "Student.h"
#include "Faculty.h"
#include "GenLinkedList.h"
#include "RollBack.h"
using namespace std;

class Database{
public:

  GenBST<Student> *studentTree;
  GenBST<Faculty> *facultyTree;
  RollBack *rb;

  Database();
  Database(GenBST<Student> *sTree, GenBST<Faculty> *fTree);
  ~Database();

  void setRb(RollBack *rbk);
  bool refIntCheck(string treeType, int id);

  void printTree(string treeType);
  void displayInfo(string treeType, int id);
  void displayRelatedInfo(string treeType, int id);

  void insert(string treeType, int id, string name, string level);
  void deletePerson(string treeType, int id);
  void changeAdvisor(int studID, int facID, int group);
  void changeAdvisor(int studID, int facID);
  void removeAdvisee(int facID, int studID);

};
Database::Database(){

}
Database::Database(GenBST<Student> *sTree, GenBST<Faculty> *fTree){
  studentTree = sTree;
  facultyTree = fTree;
}
Database::~Database(){

}
void Database::setRb(RollBack *rbk) {
  rb = rbk;
}

bool Database::refIntCheck(string treeType, int id){
  if (treeType == "Student"){
    return studentTree->personSearch(id);
  }
  if (treeType == "Faculty"){
    return facultyTree->personSearch(id);
  }
}

void Database::printTree(string treeType){
  if (treeType == "Student"){
    studentTree->printPersonTree(studentTree->root);
  }
  if (treeType == "Faculty"){
    facultyTree->printPersonTree(facultyTree->root);
  }
}

void Database::displayInfo(string treeType, int id){
  if (treeType == "Student"){
    studentTree->retrieve(id).displayData();
  }else if (treeType == "Faculty"){
    facultyTree->retrieve(id).displayData();
  }
}

void Database::displayRelatedInfo(string treeType, int id){
  if (treeType == "Student"){
    displayInfo("Faculty",studentTree->retrieve(id).facultyID);
  } else if (treeType == "Faculty"){
    GenLinkedList<int> *studentList = facultyTree->retrieve(id).studentList;
    for (int i = 0; i< studentList->getSize(); i++){
      int studID = studentList->getElement(i);
      studentTree->retrieve(studID).displayData();
    }
  }
}

void Database::insert(string treeType, int id, string name, string level){
  bool repID = false;
  while (!repID){
    if (!refIntCheck(treeType, id)) {
      // Get the rollback group number, to group together all of these transactions for rollback
      int group = rb->getLastGroup() + 1;

      if (treeType == "Student"){
        int facultyID;
        double gpa;
        bool go = true;
        while (go) {
          cout << "Insert the student's GPA: " << endl;
          cin >> gpa;
          cout << "Insert the student's faculty advisor's ID: " << endl;
          cin >> facultyID;
          if (refIntCheck("Faculty",facultyID)){
            Student stud(id,name,level,gpa,facultyID);
            // Insert the student record, keep rollback info
            studentTree->insert(stud);
            rb->store(group,"insert","Student",stud);
            // Update faculty with new advisee, keep rollback info
            Faculty facu(facultyTree->retrieve(facultyID));
            GenLinkedList<int> *tempStudList = new GenLinkedList<int>();
            for (int i = 0 ; i < facu.studentList->getSize() ; i++){
              tempStudList->insertBack(facu.studentList->getElement(i));
            }
            Faculty tempFac(facu.id,facu.name,facu.level,tempStudList);
            rb->store(group,"update","Faculty",tempFac);
            facultyTree->retrieve(facultyID).addAdvisee(id);
            go = false;
          } else{
            cout << "Faculty ID not found in database. Please try again." << endl;
          }
        }
      } else if (treeType == "Faculty"){
        string studID;
        int studentID;
        GenLinkedList<int> *studentList = new GenLinkedList<int>();
        bool go = true;
        while (go){
          cout << "Insert one of the faculty's student ID. If you are done inserting student IDs, please input 'Exit': " << endl;
          cin >> studID;
          if (studID != "Exit"){
            studentID = std::stoi(studID);
            if (refIntCheck("Student",studentID)){
              Student stud = studentTree->retrieve(studentID);
              int oldFacID = stud.getFacultyID();
              facultyTree->retrieve(oldFacID).removeAdvisee(studentID);
              studentList->insertBack(studentID);
              // Update the student record, keep rollback info
              rb->store(group,"update","Student",stud);
              stud.setFacultyID(id);
              studentTree->deleteNode(stud);
              studentTree->insert(stud);
            } else cout << "Student ID not found in database. Please try again." << endl;
          } else go = false;
        }
        Faculty facu(id, name, level, studentList);
        // Insert the faculty record, keep rollback info
        facultyTree->insert(facu);
        rb->store(group,"insert","Faculty",facu);
      }
      repID = true;
    } else {
      cout << "This " << treeType << " ID has already been assigned. Please try again." << endl;
      cout << "Input the new " << treeType << " ID:" << endl;
      cin >> id;
    }
  }
}

void Database::deletePerson(string treeType, int id){
  bool go = true;
  while (go){
    if(refIntCheck(treeType,id)){
      // Get the rollback group number, to group together all of these transactions for rollback
      int group = rb->getLastGroup() + 1;

      if (treeType == "Student"){
        Student stud = studentTree->retrieve(id);
        Faculty facu = facultyTree->retrieve(stud.facultyID);
        GenLinkedList<int> *tempStudList = new GenLinkedList<int>();
        for (int i = 0 ; i < facu.studentList->getSize() ; i++){
          tempStudList->insertBack(facu.studentList->getElement(i));
        }
        Faculty tempFac(facu.id,facu.name,facu.level,tempStudList);
        // Update the faculty record, keep rollback info
        rb->store(group,"update","Faculty",tempFac);
        facu.removeAdvisee(id);
        // Delete the student record, keep rollback info
        rb->store(group,"delete","Student",stud);
        studentTree->deleteNode(stud);
      } else if (treeType == "Faculty"){
        int newID;
        Faculty facu = facultyTree->retrieve(id);
        GenLinkedList<int> *studentList = facu.studentList;
        int size = studentList->getSize();
        for (int i = 0; i < size; i++){
          int studID = studentList->getElement(0);
          Student stud = studentTree->retrieve(studID);
          cout << "Student with ID " << studID << " will need a new advisor. Please input the new advisor ID: " << endl;
          cin >> newID;
          bool newIDExists = false;
          while (!newIDExists){
            if (refIntCheck("Faculty",newID)){
              // Update the student record, keep rollback info
              rb->store(group,"update","Student",stud);
              changeAdvisor(studID, newID, group);
              newIDExists = true;
            } else {
              cout << "New faculty ID was not found in database. Please try again." << endl;
              cout << "Input new faculty ID: " << endl;
              cin >> newID;
            }
          }
        }
        // Delete the faculty record, keep rollback info
        rb->store(group,"delete","Faculty",facu);
        facultyTree->deleteNode(facu);
      }
      go = false;
    } else{
      if (treeType == "Faculty"){
        cout << "Faculty ID not found in database. Please try again." << endl;
        cout << "Input the new Faculty ID: " << endl;
        cin >> id;
      } else if (treeType == "Student"){
        cout << "Student ID not found in database. Please try again." << endl;
        cout << "Input the new Student ID: " << endl;
        cin >> id;
      }
    }
  }
}

// Overload changeAdvisor so we can call it either with our without a rollback group number
void Database::changeAdvisor(int studID, int facID){
  int group = rb->getLastGroup() + 1;     // Get the rollback group number, to group together all of these transactions for rollback
  changeAdvisor(studID, facID, group);
}
void Database::changeAdvisor(int studID, int facID, int group){
  bool go = true;
  while (go){
    if (refIntCheck("Student",studID) && refIntCheck("Faculty",facID)){
      Student stud(studentTree->retrieve(studID));

      Faculty facu1(facultyTree->retrieve(facID));
      GenLinkedList<int> *tempStudList1 = new GenLinkedList<int>();
      for (int i = 0 ; i < facu1.studentList->getSize() ; i++){
        tempStudList1->insertBack(facu1.studentList->getElement(i));
      }
      Faculty tempFac1(facu1.id,facu1.name,facu1.level,tempStudList1);

      Faculty facu2(facultyTree->retrieve(stud.facultyID));
      GenLinkedList<int> *tempStudList2 = new GenLinkedList<int>();
      for (int i = 0 ; i < facu2.studentList->getSize() ; i++){
        tempStudList2->insertBack(facu2.studentList->getElement(i));
      }
      Faculty tempFac2(facu2.id,facu2.name,facu2.level,tempStudList2);
      // Update the student and faculty records, keep rollback info
      rb->store(group,"update","Student",stud);
      rb->store(group,"update","Faculty",tempFac1);
      rb->store(group,"update","Faculty",tempFac2);
      facultyTree->retrieve(stud.facultyID).removeAdvisee(studID);
      stud.setFacultyID(facID);
      facultyTree->retrieve(facID).addAdvisee(studID);
      studentTree->deleteNode(stud);
      studentTree->insert(stud);
      go = false;
    } else {
        cout << "Either the student ID or the faculty ID was not found in the database. Please try again." << endl;
        cout << "Input the student ID: " << endl;
        cin >> studID;
        cout << "Input the faculty ID: " << endl;
        cin >> facID;
    }
  }
}

void Database::removeAdvisee(int facID, int studID){

  // Get the rollback group number, to group together all of these transactions for rollback
  int group = rb->getLastGroup() + 1;

  bool go = true;
  int newID;
  while (go){
    if (refIntCheck("Student",studID) && refIntCheck("Faculty",facID)){
      Faculty facu(facultyTree->retrieve(facID));
      GenLinkedList<int> *tempStudList = new GenLinkedList<int>();
      for (int i = 0 ; i < facu.studentList->getSize() ; i++){
        tempStudList->insertBack(facu.studentList->getElement(i));
      }
      Faculty tempFac(facu.id,facu.name,facu.level,tempStudList);
      Student stud(studentTree->retrieve(studID));
      // Update the faculty record, keep rollback info
      rb->store(group,"update","Faculty",tempFac);
      facultyTree->retrieve(facID).removeAdvisee(studID);
      bool validAdvisor = false;
      while (!validAdvisor){
        cout << "Student with ID " << stud.id << " will need a new advisor. Please input the new advisor ID: " << endl;
        cin >> newID;
        if (refIntCheck("Faculty",newID)){
          Faculty facu2(facultyTree->retrieve(newID));
          GenLinkedList<int> *tempStudList2 = new GenLinkedList<int>();
          for (int i = 0 ; i < facu2.studentList->getSize() ; i++){
            tempStudList2->insertBack(facu2.studentList->getElement(i));
          }
          Faculty tempFac2(facu2.id,facu2.name,facu2.level,tempStudList2);
          rb->store(group,"update","Faculty",tempFac2);
          facu2.addAdvisee(studID);

          validAdvisor = true;
        } else cout << "Faculty ID not found in database. Please try again." << endl;
      }
      // Update the student record, keep rollback info
      rb->store(group,"update","Student",stud);
      stud.setFacultyID(newID);
      facultyTree->deleteNode(facu);
      facultyTree->insert(facu);
      studentTree->deleteNode(stud);
      studentTree->insert(stud);
      go = false;
    } else {
      cout << "Either the student ID or the faculty ID was not found in the database. Please try again." << endl;
      cout << "Input the faculty ID: " << endl;
      cin >> facID;
      cout << "Input the student ID: " << endl;
      cin >> studID;
    }
  }
}
