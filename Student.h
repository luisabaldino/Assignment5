#pragma once
#include <iostream>
#include <string>

using namespace std;

class Student{

public:

  int id;
  string name;
  string level;
  int facultyID;

  Student();
  Student(int idNum, string nome, string nivel, int fID);
  Student(const Student &stupidcopyconstructor);
  ~Student();

  void deepCopy(const Student &stupidcopyconstructor);
  Student& operator=(const Student &stupidcopyconstructor);

  int getFacultyID();
  void setFacultyID(int id);

  bool operator == (Student stud);
  bool operator != (Student stud);
  bool operator > (Student stud);
  bool operator < (Student stud);

  void displayData();
  string serialize();

};

Student::Student(){

}

int Student::getFacultyID(){
  return facultyID;
}

void Student::setFacultyID(int id){
  facultyID = id;
}

Student::Student(int idNum, string nome, string nivel, int fID){
  id = idNum;
  name = nome;
  level = nivel;
  facultyID = fID;
}

Student::Student(const Student &stupidcopyconstructor) {
  deepCopy(stupidcopyconstructor);
};

Student::~Student(){
}

void Student::deepCopy(const Student &stupidcopyconstructor) {
  id = stupidcopyconstructor.id;
  name = stupidcopyconstructor.name;
  level = stupidcopyconstructor.level;
  facultyID = stupidcopyconstructor.facultyID;
}

Student& Student::operator=(const Student &stupidcopyconstructor) {
  if (this != &stupidcopyconstructor) deepCopy(stupidcopyconstructor);    // check for self-assignment before doing deep copy
  return *this;
}

bool Student::operator==(Student stud){
  return (id == stud.id);
}
bool Student::operator!=(Student stud){
  return (id != stud.id);
}
bool Student::operator > (Student stud){
  return (id > stud.id);
}
bool Student::operator < (Student stud){
  return (id < stud.id);
}

void Student::displayData(){
  cout << "Student ID: " << id << endl;
  cout << "Student Name: " << name << endl;
  cout << "Student Level: " << level << endl;
  cout << "Student Faculty ID: " << facultyID << endl;
}

string Student::serialize(){
  string serialized = "student:" + std::to_string(id) + ":" + name + ":" + level + ":" + std::to_string(facultyID) + "\n";
  return serialized;
}
