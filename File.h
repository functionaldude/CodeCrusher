//
//  File.h
//  LineCounter2
//
//  Created by Zoltán Sasvári on 28.04.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#ifndef __LineCounter2__File__
#define __LineCounter2__File__

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <regex>
#include <mutex>
#include "Dialogs.h"
#include "Database.h"
#include "Algorithms.h"

class Database;

class File {
  
private:
  std::string path;
  std::string filename;
  std::string type;
  size_t size;
  unsigned int lines_code;
  unsigned int lines_comments;
  bool writeable;
  int status;
  unsigned int cs_error;
  unsigned int classes;
  
public:
  File(std::string input_filename);
  void countFile();
  void printStats();
  void readFileType();
  void processFile();
  void writeToFile(std::vector<std::string> lines, unsigned long line_number, char mode);
  void checkCCodingStandard(char mode);
  void checkHCodingStandard(char mode);
  void countClasses();
  void checkOriginality(std::string filename2);
  //getters
  unsigned int getCode();
  unsigned int getComments();
  std::string getStatus();
  std::string getPath();
  std::string getName();
  bool getWriteStatus();
  std::string getType();
  //setters
  void setWrite(bool input);
  void setFile(File input_file);
  void setName();
};

bool checkIfFileExists(std::string filename);
bool checkIfFileRegistered(std::string filename, Database * db);
size_t getSize(std::string filename);

#endif /* defined(__LineCounter2__File__) */
