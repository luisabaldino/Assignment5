#pragma once
#include <iostream>
#include <string>

using namespace std;

class Student{

public:

  int id;
  string name;
  string level;
  double gpa;
  int facultyID;

  Student();
  Student(int idNum, string nome, string nivel,double nota, int fID);
  ~Student();

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

Student::Student(int idNum, string nome, string nivel,double nota, int fID){
  id = idNum;
  name = nome;
  level = nivel;
  gpa = nota;
  facultyID = fID;
}

Student::~Student(){
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
//Displays all student information
void Student::displayData(){
  cout << "Student ID: " << id << endl;
  cout << "Student Name: " << name << endl;
  cout << "Student Level: " << level << endl;
  cout << "Student GPA: " << gpa << endl;
  cout << "Student Faculty ID: " << facultyID << endl;
}
//Converts student tree to standard txt output
string Student::serialize(){
  string serialized = "student:" + std::to_string(id) + ":" + name + ":" + level + ":" + std::to_string(gpa) + ":" + std::to_string(facultyID) + "\n";
  return serialized;
}
