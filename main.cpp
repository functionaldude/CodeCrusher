//
//  main.cpp
//  LineCounter2
//
//  Created by Zoltán Sasvári on 28.04.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <regex>
#include "ThreadFunctions.h"
#include "Database.h"
#include "Dialogs.h"
#include "File.h"

using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::vector;
using std::thread;
using std::mutex;
using std::ref;

int main(int argc, const char * argv[]) {
//  Database activedb(argv[1], true);
  if (argc != 2) {
    cout << "Insufficient Parameters! Read the README." << endl;
    return -1;
  }
  Database* activedb;
  try {
    activedb = new Database(argv[1], true);
  } catch (std::bad_alloc& ba) {
    cout << "Bad alloc at Project: " << ba.what() << endl;
    return 0;
  }
    
  mutex access_cout;
  thread counter;
  thread writer;
  bool autosaver_state = true;
  thread autosaver(MULTIautosave, ref(*activedb), ref(activedb->autosaveable), ref(autosaver_state));
  string input;
  vector<string> input_args;
  
  while (1) {
    access_cout.unlock();
    input.clear();
    input.shrink_to_fit();
    input_args.clear();
    input_args.shrink_to_fit();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    access_cout.lock();
    cout << "> ";
    getline(cin, input);
    input_args = splitArgs(input);
    
    if (input_args[0] == "add") {
      if (input_args[1] == "file") {
        cout << "Specify file location: " << endl;
        cin >> input;
        activedb->addFiles(input);
      }
      continue;
    }
    if (input_args[0] == "remove") {
      if (input_args[1] == "file") {
        cout << "Specify file: " << endl;
        cin >> input;
        activedb->removeFile(input);
      }
      continue;
    }
    if (input_args[0] == "process") {
      if (input_args[1] == "db") {
        if (counter.joinable()) {
          counter.join();
        }
        counter = thread(MULTIprocessDB, ref(*activedb), ref(access_cout));
        continue;
      }
      if (checkIfFileRegistered(input_args[1], activedb) == false) {
        cout << "No such file" << endl;
        continue;
      } else {
        counter = thread(MULTIprocessFile, ref(*activedb), ref(input_args[1]), ref(access_cout));
        continue;
      }
    }
    if (input_args[0] == "print"){
      if (input_args[1] == "all") {
        activedb->printFiles();
        continue;
      }
      if (input_args[1] == "db") {
        activedb->printStats();
        continue;
      }
      //engines
      if (input_args[2] == "cs") {
        if (activedb->getFile(input_args[1])->getType() == "C") {
          activedb->getFile(input_args[1])->checkCCodingStandard('l');
        }
        if (activedb->getFile(input_args[1])->getType() == "C++") {
          activedb->getFile(input_args[1])->checkCCodingStandard('l');
        }
        if (activedb->getFile(input_args[1])->getType() == "h") {
          activedb->getFile(input_args[1])->checkHCodingStandard('l');
        }
        continue;
      }
      if (input_args[2] == "pl") {
        cout << "Specify the suspected file (absolute path):" << endl;
        getline(cin, input);
        activedb->getFile(input_args[1])->checkOriginality(input);
        continue;
      }
      if (checkIfFileRegistered(input_args[1], activedb) == false) {
        cout << "No such file" << endl;
        continue;
      }
      activedb->getFile(input_args[1])->printStats();
      continue;
    }
    if (input_args[0] == "set") {
      if (input_args[1] == "db") {
        if (input_args[2] == "name") {
          cout << "New name";
          getline(cin, input);
          activedb->setName(input);
          cout << "New name: " << activedb->getName() << endl;
          continue;
        }
      }
      if (checkIfFileRegistered(input_args[1], activedb) == false) {
        cout << "No such file" << endl;
        continue;
      }
      if (input_args[2] == "w") {
        bool input = false;
        if (input_args[3] == "true") {
          input = true;
        }
        if (input_args[3] == "false") {
          input = false;
        }
        if (input_args[3] != "true" && input_args[3] != "false") {
          cout << "Specify value!" << endl;
          continue;
        }
        activedb->getFile(input_args[1])->setWrite(input);
      }
      continue;
    }
    if (input_args[0] == "list") {
      cout << "Files:" << endl;
      activedb->printFilenames();
      continue;
    }
    if (input_args[0] == "save") {
      activedb->save();
      cout << "Saved!" << endl;
      continue;
    }
    if (input_args[0] == "write") {
      if (checkIfFileRegistered(input_args[1], activedb) == false) {
        cout << "No such file" << endl;
        continue;
      }
      if (writer.joinable()) {
        writer.join();
      }
      unsigned long line_number = 0;
      unsigned long line_index = 0;
      string line;
      vector<string> lines;
      char write_mode;
      cout << "Specify write mode (w/a): ";
      cin >> write_mode;
      cout << "How many separate lines? " << endl;
      cin >> line_index;
      flushInput();
      while (lines.size() < line_index) {
        cout << "Line: " << lines.size() + 1 << endl;
        getline(cin, line);
        lines.push_back(line);
      }
      cout << "How many line would you like to generate?" << endl;
      cin >> line_number;
      cout << "Using lines:" << endl;
      for (int counter = 0; counter < lines.size(); counter++) {
        cout << lines[counter] << endl;
      }
      writer = thread(MULTIwriteFile, std::ref(*activedb), input_args[1], lines, line_number, write_mode, ref(access_cout));
      line.clear();
      lines.clear();
      line.shrink_to_fit();
      lines.shrink_to_fit();
      continue;
    }
    if (input_args[0] == "quit") {
      cout << "Terminating threads" << endl;
      autosaver_state = false;
      activedb->autosaveable.unlock();
      autosaver.join();
      if (counter.joinable()) {
        counter.join();
      }
      if (writer.joinable()) {
        writer.join();
      }
      delete activedb;
      return 0;
    }
    if (input_args[0] == "disable") {
      if (input_args[1] == "autosave") {
        autosaver_state = false;
        activedb->autosaveable.unlock();
        autosaver.join();
      }
      continue;
    }
    if (input_args[0] == "help" || input_args[0] == "h") {
      printHelp();
      continue;
    }
    if (input_args[0] == "") {
      continue;
    }
    cout << "Unknown cmd" << endl;
  }
}