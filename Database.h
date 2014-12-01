//
//  Database.h
//  CodeCrusher
//
//  Created by Zoltán Sasvári on 29.04.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#ifndef __CodeCrusher__Database__
#define __CodeCrusher__Database__

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "File.h"

class File;

class Database{
  
private:
  std::string name;
  std::vector<File> files;
  std::string savefile;
  unsigned int number_of_files;
  unsigned int lines_code;
  unsigned int lines_comments;
  bool writable;
  bool saved;
  std::mutex access_db;
  
public:
  Database(std::string dbfilename, bool file_writeable);
  std::mutex autosaveable;
  void addFiles(std::string input_name);
  void removeFile(std::string input_name);
  void processDB();
  void processFile (std::string file);
  void printStats();
  void printFiles();
  void printFilenames();
  void save();
  void freeSpace();
  
  //getters
  bool getSaveStatus();
  bool getWriteStatus();
  File* getFile(std::string searched);

  //setters
  void setName(std::string);
};

#endif /* defined(__CodeCrusher__Database__) */
