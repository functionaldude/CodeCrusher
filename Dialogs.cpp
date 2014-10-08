//
//  Dialogs.cpp
//  LineCounter2
//
//  Created by Zoltán Sasvári on 28.04.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#include "Dialogs.h"
#include <vector>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::vector;

void printHelp () {
  cout << "Usage: [program] [project_file]" << endl;
  cout << "For more information read the README." << endl;
}

void printSeparator () {
  cout << "---------------------------" << endl;
}

vector<string> splitArgs(string s) {
  vector<string> result;
  string word;
  std::stringstream stream(s);
  
  while(stream >> word)
  {
    result.push_back(word);
  }
  result.push_back(s);
  return result;
}

void flushInput(){
  char current = '\0';
  while (current != '\n') {
    current = getchar();
  }
}