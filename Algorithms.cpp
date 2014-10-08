//
//  Algorithms.cpp
//  CodeCrusher
//
//  Created by Zoltán Sasvári on 13.05.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#include "Algorithms.h"

using std::string;
using std::regex;
using std::vector;

bool checkValidVariable(std::string input){
  if (string::npos!=input.find("(") ||
      string::npos!=input.find(")") ||
      string::npos!=input.find("{") ||
      string::npos!=input.find("}") ||
      string::npos!=input.find("using") )
  {
    return false;
  } else {
    return true;
  }
}

bool checkValidVariableType(string input){
  if (string::npos!=input.find("int") ||
      string::npos!=input.find("void") ||
      string::npos!=input.find("char") ||
      string::npos!=input.find("string") ||
      string::npos!=input.find("float") ||
      string::npos!=input.find("double") ||
      string::npos!=input.find("long") ||
      string::npos!=input.find("vector") ||
      string::npos!=input.find("bool") ||
      string::npos!=input.find("mutex") ||
      string::npos!=input.find("size_t") ||
      string::npos!=input.find("ifstream") ||
      string::npos!=input.find("ofstream") ||
      string::npos!=input.find("fstream"))
  {
    return true;
  } else {
    return false;
  }
}

bool checkMutexStatus(std::mutex* input){
  if (input->try_lock()) {
    input->unlock();
    return false;
  } else {
    return true;
  }
}