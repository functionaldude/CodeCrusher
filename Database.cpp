//
//  Database.cpp
//  CodeCrusher
//
//  Created by Zoltán Sasvári on 29.04.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#include "Database.h"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::thread;

Database::Database(string dbfilename, bool file_writeable) :
  savefile(dbfilename),
  writable(file_writeable)
{
  access_db.lock();
  std::fstream dbfile;
  dbfile.open(savefile);
  if (!dbfile) {
    dbfile.close();
    dbfile.open(savefile, std::ios::out | std::ios::app);
    cout << "Creating new project!" << endl;
    cout << "Name: ";
    cin >> name;
    dbfile << name << endl;
    dbfile << "0" << endl;
    dbfile << "0" << endl;
    dbfile << "0" << endl;
    saved = true;
    access_db.unlock();
    return;
  }
  string line;
  dbfile >> name;
  dbfile >> number_of_files;
  dbfile >> lines_code;
  dbfile >> lines_comments;
  unsigned int file_counter = 0;
  while (1) {
    dbfile >> line;
    if (!dbfile) {
      break;
    }
    file_counter++;
    try {
      files.push_back(File(line));
    } catch (std::bad_alloc& ba) {
      cout << "Bad alloc at File(" << file_counter << ") :" << ba.what() << endl;
    }
  }
  if (number_of_files != file_counter) {
    cout << "Project corrupt!" << endl;
    number_of_files = file_counter;
    cout << "Project fixed!" << endl;
    save();
  }
  dbfile.close();
  files.shrink_to_fit();
  saved = true;
  autosaveable.try_lock();
  cout << "Project " << name << " loaded!" << endl;
  access_db.unlock();
}

void Database::addFiles(string input_name){
  if (writable == false) {
    cout << "Error: Project is read-only" << endl;
    return;
  }
  if (access_db.try_lock()) {
    files.push_back(File(input_name));
    number_of_files++;
    cout << "File " << input_name << " added" << endl;
    saved = false;
    autosaveable.unlock();
    access_db.unlock();
  } else {
    cout << "Error: Project file is in use" << endl;
  }
}

void Database::removeFile(string searched){
  int counter = 0;
  string current;
  while (std::string::npos == current.find(searched)) {
    current = files[counter].getPath();
    counter++;
    if (counter >= files.size() + 1) {
      cout << "No such file in the project" << endl;
      return;
    }
  }
  if (access_db.try_lock()) {
    cout << "Deteled file: " << files[counter - 1].getPath() << endl;
    number_of_files--;
    files.erase(files.begin() + (counter - 1));
  } else {
    cout << "Error: Project file is in use" << endl;
  }
}

void Database::processDB(){
  access_db.lock();
  unsigned int code = 0;
  unsigned int comment = 0;
  lines_code = 0;
  lines_comments = 0;
  
  thread worker1;
  thread worker2;
  thread worker3;
  thread worker4;
  
  for (int counter = 0; counter < files.size();) {
    try {
      worker1 = thread(&File::processFile, &files[counter]);
      counter++;
      if (counter >= files.size()) {
        break;
      }
      worker2 = thread(&File::processFile, &files[counter]);
      counter++;
      if (counter >= files.size()) {
        break;
      }
      worker3 = thread(&File::processFile, &files[counter]);
      counter++;
      if (counter >= files.size()) {
        break;
      }
      worker4 = thread(&File::processFile, &files[counter]);
      counter++;
    } catch (std::length_error) {}
    if (worker1.joinable()) {
      worker1.join();
    }
    if (worker2.joinable()) {
      worker2.join();
    }
    if (worker3.joinable()) {
      worker3.join();
    }
    if (worker4.joinable()) {
      worker4.join();
    }
  }
  
  for (int counter = 0; counter < files.size(); counter++){
//    files[counter].processFile();
    code += files[counter].getCode();
    comment += files[counter].getComments();
  }
  lines_code = code;
  lines_comments = comment;
  saved = false;
  autosaveable.unlock();
  access_db.unlock();
  if (worker1.joinable()) {
    worker1.join();
  }
  if (worker2.joinable()) {
    worker2.join();
  }
  if (worker3.joinable()) {
    worker3.join();
  }
  if (worker4.joinable()) {
    worker4.join();
  }
}

void Database::printFiles(){
  for (int counter = 0; counter < files.size(); counter++) {
    files[counter].printStats();
  }
}

void Database::printFilenames(){
  int counter = 0;
  for (counter = 0; counter < files.size(); counter++) {
    cout << counter + 1 << ". " << files[counter].getName() << endl;
  }
  if (counter == 0) {
    cout << "Database empty. Add some files!" << endl;
  }
}

void Database::save(){
  if (writable == false) {
    return;
  }
  access_db.lock();
  std::ofstream dbfile;
  dbfile.open(savefile, std::ios::out);
  dbfile << name << endl;
  dbfile << number_of_files << endl;
  dbfile << lines_code << endl;
  dbfile << lines_comments << endl;
  for (int counter = 0; counter < files.size(); counter++){
    dbfile << files[counter].getPath() << endl;
  }
  saved = true;
  autosaveable.try_lock();
  dbfile.close();
  access_db.unlock();
}

File * Database::getFile(string searched){
  int counter = 0;
  string current;
  while (std::string::npos == current.find(searched)){
    current = files[counter].getPath();
    counter++;
    if (counter >= files.size() + 1) {
      return NULL;
    }
  }
  return &files[counter - 1];
}

void Database::printStats(){
  cout << "Project name: " << name << endl;
  cout << "Files: " << number_of_files << endl;
  cout << "Lines of code: " << lines_code << endl;
  cout << "Lines of comment: " << lines_comments << endl;
  cout << "Files: " << endl;
  printFilenames();
}

bool Database::getSaveStatus(){
  return saved;
}

bool Database::getWriteStatus(){
  return writable;
}

void Database::freeSpace(){
  access_db.lock();
  name.shrink_to_fit();
  savefile.shrink_to_fit();
  files.shrink_to_fit();
  access_db.unlock();
}

void Database::setName(std::string input){
  name = input;
}

std::string Database::getName(){
  return name;
}