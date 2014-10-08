//
//  ThreadFunctions.cpp
//  CodeCrusher
//
//  Created by Zoltán Sasvári on 06.05.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#include "ThreadFunctions.h"

using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::vector;
using std::mutex;

void MULTIprocessDB(Database& activedb, mutex& mtx_cout){
  activedb.processDB();
  mtx_cout.lock();
  cout << "Processing completed!" << endl;
  mtx_cout.unlock();
  return;
}

void MULTIwriteFile(Database& activedb, string filename, vector<string> lines, unsigned long line_multiplier, char mode, mutex& mtx_cout){
  activedb.getFile(filename)->writeToFile(lines, line_multiplier, mode);
  if (activedb.getFile(filename)->getWriteStatus() == false) {
    return;
  }
  mtx_cout.lock();
  cout << "Write completed!" << endl;
  mtx_cout.unlock();
  filename.clear();
  filename.shrink_to_fit();
  lines.clear();
  lines.shrink_to_fit();
  return;
}

void MULTIprocessFile(Database& activedb, string& filename, mutex& mtx_cout){
  activedb.getFile(filename)->processFile();
  mtx_cout.lock();
  cout << "Processing completed!" << endl;
  mtx_cout.unlock();
  return;
}

void MULTIautosave(Database& activedb, mutex& save, bool& state){
  while (state) {
    if (activedb.getWriteStatus() == false) {
      state = false;
      break;
    }
    save.lock();
    if (state == false) {
      break;
    }
    activedb.freeSpace();
    activedb.save();
  }
}