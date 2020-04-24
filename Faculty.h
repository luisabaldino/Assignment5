#pragma once
#include <iostream>
#include <string.h>
#include <sstream>
#include "GenLinkedList.h"

using namespace std;

class Faculty{

public:

  int id;
  string name;
  string level;
  GenLinkedList<int> *studentList;

  Faculty();
  Faculty(int idNum, string nome, string nivel, GenLinkedList<int> *sList);
  Faculty(int idNum, string nome, string nivel);
  ~Faculty();

  void setStudentList(string stlist);
  bool searchStudentList(int id);
  void removeAdvisee(int studID);
  void addAdvisee(int studID);

  bool operator == (Faculty facu);
  bool operator != (Faculty facu);
  bool operator > (Faculty facu);
  bool operator < (Faculty facu);

  void displayData();
  string serialize();

};

Faculty::Faculty(){
}
Faculty::Faculty(int idNum, string nome, string nivel, GenLinkedList<int> *sList){
  id = idNum;
  name = nome;
  level = nivel;
  studentList = sList;
}
Faculty::Faculty(int idNum, string nome, string nivel){
  id = idNum;
  name = nome;
  level = nivel;
  studentList = new GenLinkedList<int>();
}
Faculty::~Faculty(){
}

void Faculty::setStudentList(string stlist){
  //assumes stlist is a colon-delimited string of IDs
  int size = stlist.length();
  char str[size+1];
  strcpy(str,stlist.c_str());
  char *token;
  token = strtok(str,":");
  while (token!=NULL){
    studentList->insertBack(std::stoi(token));
    token = strtok(NULL,":");
  }
}

//Returns true if student ID is found in studentList
bool Faculty::searchStudentList(int id){
  for (int i = 0; i < studentList->size; i++){
    if (studentList->getElement(i) == id) return true;
  }
  return false;
}
//Adds student to studentList
void Faculty::addAdvisee(int studID){
  if (!searchStudentList(studID)){
    studentList->insertBack(studID);
  }
}
//Removes student from studentList
void Faculty::removeAdvisee(int studID){
  studentList->remove(studID);
}

bool Faculty::operator==(Faculty facu){
  return (id == facu.id);
}
bool Faculty::operator!=(Faculty facu){
  return (id != facu.id);
}
bool Faculty::operator > (Faculty facu){
  return (id > facu.id);
}
bool Faculty::operator < (Faculty facu){
  return (id < facu.id);
}
//Displays faculty data
void Faculty::displayData(){
  cout << "Faculty ID: " << id << endl;
  cout << "Faculty Name: " << name << endl;
  cout << "Faculty Level: " << level << endl;
  cout << "Faculty's Student IDs: " << endl;
  for (int i = 0; i < studentList->getSize(); i++){
    cout << studentList->getElement(i) << endl;
  }
}
//Converts faculty tree to standard txt output
string Faculty::serialize(){
  string serialized = "faculty:" + std::to_string(id) + ":" + name + ":" + level;
  for (int i = 0; i < studentList->getSize(); i++){
    serialized += ":" + std::to_string(studentList->getElement(i));
  }
  serialized += "\n";
  return serialized;
}
