#include <iostream>
#include "Database.h"
#include "Student.h"
#include "Faculty.h"
#include "GenBST.h"
#include <fstream>
#include <string.h>
#include <stdlib.h>
using namespace std;

class UX{
public:
  GenBST<Student> *masterStudent;
  GenBST<Faculty> *masterFaculty;
  Database *db;
  RollBack *rb;
  int option;

  ifstream inFile;

  UX();
  ~UX();
  void showMenu();
  void processOptions();
  void loadDB(string fileName);

};
UX::UX(){
  this->masterStudent = new GenBST<Student>();
  this->masterFaculty = new GenBST<Faculty>();
  db = new Database(masterStudent,masterFaculty);
  rb = new RollBack(masterStudent,masterFaculty);
  db->setRb(rb);
  loadDB("studentTable");
  loadDB("facultyTable");
}
UX::~UX(){
}
void UX::showMenu(){
//Print menu options
cout << "=========================================================================" << endl;
cout << "1. Print all students and their information (sorted by ascending id #)." << endl;
cout << "2. Print all faculty and their information (sorted by ascending id #)." << endl;
cout << "3. Find and display student information given the students id." << endl;
cout << "4. Find and display faculty information given the faculty id." << endl;
cout << "5. Given a student’s id, print the name and info of their faculty advisor." << endl;
cout << "6. Given a faculty id, print ALL the names and info of his/her advisees." << endl;
cout << "7. Add a new student." << endl;
cout << "8. Delete a student given the id." << endl;
cout << "9. Add a new faculty member." << endl;
cout << "10. Delete a faculty member given the id." << endl;
cout << "11. Change a student’s advisor given the student id and the new faculty id." << endl;
cout << "12. Remove an advisee from a faculty member given the ids." << endl;
cout << "13. Rollback." << endl;
cout << "14. Exit." << endl;

//Read user option
cout << "Please input the number of the option you would like to complete:" << endl;
cin >> option;
processOptions();
}

void UX::processOptions(){
// Call corresponding db functions
bool exit = false;
while (!exit){
  if (option == 1) {
      db->printTree("Student");
    }else if (option == 2){
      db->printTree("Faculty");
    }else if (option == 3){
      int id;
      cout << "Input the student ID: " << endl;
      cin >> id;
      db->displayInfo("Student",id);
    }else if (option == 4){
      int id;
      cout << "Input the faculty ID: " << endl;
      cin >> id;
      db->displayInfo("Faculty",id);
    }else if (option == 5){
      int id;
      cout << "Input the student ID: " << endl;
      cin >> id;
      db->displayRelatedInfo("Student",id);
    }else if (option == 6){
      int id;
      cout << "Input the faculty ID: " << endl;
      cin >> id;
      db->displayRelatedInfo("Faculty",id);
    }else if (option == 7){
      string name;
      int id;
      string level;
      cout << "Input the student ID: " << endl;
      cin >> id;
      cout << "Input the student name: " << endl;
      cin >> name;
      cout << "Input the student level: " << endl;
      cin >> level;
      db->insert("Student",id,name,level);
    }else if (option == 8){
      int id;
      cout << "Input the student ID: " << endl;
      cin >> id;
      db->deletePerson("Student",id);
    }else if (option == 9){
      string name;
      int id;
      string level;
      cout << "Input the faculty ID: " << endl;
      cin >> id;
      cout << "Input the faculty name: " << endl;
      cin >> name;
      cout << "Input the faculty level: " << endl;
      cin >> level;
      db->insert("Faculty",id,name,level);
    }else if (option == 10){
      int id;
      cout << "Input the faculty ID: " << endl;
      cin >> id;
      db->deletePerson("Faculty",id);
    }else if (option == 11){
      int studid;
      int facid;
      cout << "Input the student ID: " << endl;
      cin >> studid;
      cout << "Input the faculty ID: " << endl;
      cin >> facid;
      db->changeAdvisor(studid,facid);
    }else if (option == 12){
      int studid;
      int facid;
      cout << "Input the faculty ID: " << endl;
      cin >> facid;
      cout << "Input the student ID: " << endl;
      cin >> studid;
      db->removeAdvisee(facid,studid);
    }else if (option == 13){
      int steps = 0;
      cout << "Input number of transactions to undo (unlimited): " << endl;
      cin >> steps;
      rb->undo(steps);
    }else if (option == 14){
      masterStudent->saveTree("studentTable");
      masterFaculty->saveTree("facultyTable");
      exit = true;
      break;
    }
    cout << endl << endl;
    showMenu();
  }
}

void UX::loadDB(string fileName){
  string line;
  inFile.open(fileName + ".txt");
  while (!inFile.eof()){
    //For each line in given file, read info & store in variables
    getline(inFile,line);
    if(line.length()==0)break;
    int size = line.length();
    char str[size+1];
    strcpy(str,line.c_str());
    char *token;
    token = strtok(str,":");
    string treeType = token;
    token = strtok(NULL,":");
    int id = std::stoi(token);
    token = strtok(NULL,":");
    string name = token;
    token = strtok(NULL,":");
    string level = token;

    if (treeType == "student"){
      token = strtok(NULL,":");
      double gpa = std::stod(token);
      cout << gpa << endl;
      token = strtok(NULL,":");
      int facuID = std::stoi(token);
      // here we have all info to create a student
      Student stud(id,name,level,gpa,facuID);
      masterStudent->insert(stud);
    } else if (treeType == "faculty"){
      string studIDs = "";
      token = strtok(NULL,":");
      while (token!=NULL){
        studIDs += ":";
        studIDs += token;
        token = strtok(NULL,":");
      }
      // here we have all info to create a faculty
      Faculty facu(id,name,level);
      facu.setStudentList(studIDs);
      masterFaculty->insert(facu);
    }
  }
  inFile.close();
}
