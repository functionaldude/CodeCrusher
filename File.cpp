//
//  File.cpp
//  LineCounter2
//
//  Created by Zoltán Sasvári on 28.04.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#include "File.h"


using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::vector;
using std::regex;
using std::ifstream;
using std::ios;
using std::mutex;

File::File(string input_path) :
  path(input_path),
  type("unknown"),
  lines_code(0),
  lines_comments(0),
  status(0),
  writeable(false),
  cs_error(0),
  classes(0)
{
  size = getSize(path);
  readFileType();
  setName();
}

unsigned int File::getCode(){
  return lines_code;
}

unsigned int File::getComments(){
  return lines_comments;
}

string File::getType(){
  return type;
}

std::string File::getStatus(){
  switch (status) {
    case 0:
      return "Ready";
    case -1:
      return "Error";
    case 1:
      return "Analyzed";
    case -2:
      return "Not analyzable";
    case -3:
      return "Processing";
  }
  return "Ready";
}

bool File::getWriteStatus(){
  return writeable;
}

std::string File::getPath(){
  return path;
}

std::string File::getName(){
  return filename;
}

void File::setName(){
  string buffer;
  buffer = path;
  size_t position = 0;
  size_t position_last = 0;
  
  //for unix/linux/mac users
  while (true) {
    position = buffer.find('/', position + 1);
    if (position == string::npos) {
      break;
    }
    position_last = position;
  }
  
  //for Windows users
  position = 0;
  
  while (true) {
    position = buffer.find('\\', position + 1);
    if (position == string::npos) {
      break;
    }
    position_last = position;
  }
  buffer.erase(0, position_last + 1);
  filename = buffer;
}

void File::countFile() {
  std::ifstream file;
  file.open(path, std::ios::in);
  if (!file) {
    status = -1;
    return;
  }
  
  string line;
  unsigned int commented = 0;
  int counter = 0;
  unsigned int lines = 0;
  bool is_commented = false;
  while (1) {
    getline(file, line);
    if (!file) {
      break;
    }
    //taking out the one line comments
    if ((line[0] == '/') && (line[1] == '/')) {
      commented++;
      continue;
    }
    //taking out the linebreaks
    if (line[0] == '\n' || line[0] == '{' || line[0] == '}') {
      continue;
    }
    if (line[0] == ' ' || line[0] == '\t') {
      counter = 0;
      while (line[counter] == ' ' || line[counter] == '\t') {
        counter++;
      }
      if (line[counter] == '\n') {
        continue;
      }
      if (line[counter] == '/' && line[counter + 1] == '/') {
        commented++;
        continue;
      }
      if ((line[counter] == '{' || line[counter] == '}') && line[counter + 1] == '\n') {
        continue;
      }
    }
    //taking out the multiline comments
    if (std::string::npos!=line.find("/*")) {
      is_commented = true;
      commented++;
      continue;
    }
    if (std::string::npos!=line.find("*/")) {
      is_commented = false;
      commented++;
      continue;
    }
    if (is_commented == true) {
      commented++;
      continue;
    }
    lines++;
    
  }
  lines_code = lines;
  lines_comments = commented;
  status = 1;
  file.close();
}

void File::printStats(){
  printSeparator();
  if (status == -1) {
    cout << "File: " << path << " not found" << endl;
    printSeparator();
    return;
  }
  cout << "File: " << filename << endl;
  cout << "Path: " << path << endl;
  cout << "Type: " << type << endl;
  cout << "Writable: ";
  if (writeable) {
    cout << "true" << endl;
  } else {
    cout << "false" << endl;
  }
  if (lines_code != 0) {
    cout << lines_code << " lines of code" << endl;
  }
  if (lines_comments != 0) {
    cout << lines_comments << " lines of comment" << endl;
  }
  if (cs_error != 0) {
    cout << cs_error << " possible Coding Standard vialoations" << endl;
  }
  if (classes != 0) {
    cout << classes << " classes" << endl;
  }
  cout << "Size: " << size /1024 << "KB" << endl;
  cout << "Status: "<< getStatus() << endl;
  printSeparator();
}
void File::readFileType(){
  regex c("(.*)(c)");
  regex cpp("(.*)(cpp)");
  regex h("(.*)(h)");
  regex txt("(.*)(txt)");
  if (regex_search(path, cpp)) {
    type = "C++";
    return;
  }
  if (regex_search(path, c)) {
    type = "C";
    return;
  }
  if (regex_search(path, h)) {
    type = "h";
    return;
  }
  if (regex_search(path, txt)) {
    type = "Text file";
    return;
  }
  type = "unknown";
}

void File::processFile() {
  if (path == "") {
    return;
  }
  bool truefile = false;
  try {
    truefile = checkIfFileExists(path);
  }
  catch (std::length_error) {return;}
  if (truefile) {
    status = -3;
    readFileType();
    size = getSize(path);
    if (type == "C") {
      countFile();
      checkCCodingStandard('s');
      return;
    }
    if (type == "C++") {
      countFile();
      checkCCodingStandard('s');
      return;
    }
    if (type == "h") {
      countClasses();
      checkHCodingStandard('s');
      return;
    }
    status = -2;
  }
}

void File::countClasses(){
  ifstream file;
  file.open(path, std::ios::in);
  if (!file) {
    status = -1;
    return;
  }
  string line;
  unsigned int counter = 0;
  
  while (true) {
    getline(file, line);
    if (!file) {
      break;
    }
    if (string::npos!=line.find("class") && string::npos==line.find(";")) {
      counter++;
    }
  }
  status = 1;
}

size_t getSize(string path){
  size_t size = 0;
  if (checkIfFileExists(path)) {
    std::ifstream file;
    file.open(path, std::ios::binary);
    file.seekg(0, file.end);
    size = (size_t) file.tellg();
    file.seekg(0, file.beg);
    return size;
  } else {
    return 0;
  }
}

bool checkIfFileExists(string path){
  ifstream file;
  file.open(path, std::ios::binary);
  if (!file) {
    return false;
  } else {
    return true;
  }
}

bool checkIfFileRegistered(string path, Database * db){
  if (db->getFile(path) == NULL) {
    return false;
  } else {
    return true;
  }
}

void File::writeToFile(vector<string> lines, unsigned long line_number, char mode){
  cout << "File: " << path << endl;
  if (!writeable) {
    cout << "Error: File is marked as read-only!" << endl;
    return;
  }
  std::ofstream file;
  if (mode == 'w') {
    file.open(path, std::ios::out);
  }
  if (mode == 'a') {
    file.open(path, std::ios::app);
  }
  string line;
  for (int counter_line = 0; counter_line < line_number; counter_line++) {
    for (int counter_index = 0; counter_index < lines.size(); counter_index++) {
      file << lines[counter_index] << endl;
    }
  }
}

void File::setWrite(bool input){
  if (input) {
    writeable = true;
  } else {
    writeable = false;
  }
}

void File::setFile(File input_file){
  path = input_file.path;
  type = input_file.type;
  size = input_file.size;
  lines_code = input_file.lines_code;
  lines_comments = input_file.lines_comments;
  writeable = input_file.writeable;
  status = input_file.status;
}

void File::checkCCodingStandard(char mode) {
  //mode s = silent
  if (type == "unknown") {
    readFileType();
  }
  if (mode == 'l' && type != "C" && type != "C++") {
    cout << "Not a valid C/C++ file" << endl;
    return;
  }
  ifstream file;
  file.open(path, std::ios::in);
  
  int NEXT_CODE = -1;
  int CODE = 0;
  int CHAR = 1;
  int STRING = 2;
  int COMMENT_SLASH_SLASH = 3;
  
  unsigned int errors = 0;
  int character = 0;
  int prev_character = 0;
  int position_column = 1;
  int position_character = 1;
  int indentation = 0;
  int whitespace_counter = 0;
  int code_type = CODE;
  int bracket_line = 0;
  
  do
  {
    prev_character = character;
    character = file.get();
    
    if (code_type == NEXT_CODE){
      code_type = CODE;
    }
    
    if (character == '\'' && prev_character != 92){ // 92 = backslash
      if (code_type == CODE){
        code_type = CHAR;
      }
      else if (code_type == CHAR){
        code_type = NEXT_CODE;
      }
    }
    
    if (character == '"' && prev_character != 92) // 92 = backslash
    {
      if (code_type == CODE){
        code_type = STRING;
      }
      else if (code_type == STRING){
        code_type = NEXT_CODE;
      }
    }
    
    if (character == '/' && prev_character == '/'){
      code_type = COMMENT_SLASH_SLASH;
    }
    if (code_type == CODE){
      if (character == '='){
        if (prev_character != ' ' && prev_character != '=' &&
            prev_character != '!' && prev_character != '/' &&
            prev_character != '*' && prev_character != '+' &&
            prev_character != '-' && prev_character != '|' &&
            prev_character != '&' && prev_character != '<' &&
            prev_character != '>' && prev_character != '%')
        {
          if (mode != 's') {
            printf("%d:%d: whitespace before '=' operator missing\n", \
                   position_column, position_character);
          }
          errors++;
        }
      } else {
        if (prev_character == '=' && character != ' '){
          if (mode != 's') {
            printf("%d:%d: whitespace after '=' operator missing\n", \
                   position_column, position_character);
          }
          errors++;
        }
      }
      if (character == '\t') {
        if (mode != 's') {
          printf("%d:%d: tabulator found\n", position_column, position_character);
        }
        errors++;
      }
      
      if (bracket_line == 1 && character != ' ' && character != '\n') {
        if (mode != 's') {
          printf("%d: '}' or '{' in wrong position\n", position_column);
        }
        errors++;
        bracket_line = -1; // avoid multiple errors
      }
      
      if (character == '}') {
        if (indentation - position_character != 1) {
          if (mode != 's') {
            printf("%d: '}' in wrong position\n", position_column);
          }
          errors++;
        }
        indentation -= 2;
        bracket_line = 1;
      }
      if (whitespace_counter != -1) {
        if (character == ' ' || character == '\n') {
          whitespace_counter ++;
        }
        else {
          if (whitespace_counter != indentation) {
            if (mode != 's') {
              printf("%d: wrong indentation\n", position_column);
            }
            errors++;
          }
          whitespace_counter = -1; // thou shalt not count
        }
      }
      
      if (character == '{') {
        indentation += 2;
        bracket_line = 1;
        if (indentation - position_character != 1) {
          if (mode != 's') {
            printf("%d: '{' in wrong position\n", position_column);
          }
          errors++;
        }
      }
    }
    position_character ++;
    if (character == '\n') {
      if (position_character - 2 > 80){ // -2 because of ++ before and \n
        if (mode != 's') {
          printf("%d: length of line >80\n", position_column);
        }
        errors++;
      }
      position_column ++;
      position_character = 1;
      whitespace_counter = 0;
      bracket_line = 0;
      
      if (code_type == COMMENT_SLASH_SLASH) {
        code_type = CODE;
      }
    }
  }
  while (character != EOF);
  
  if (type == "C++") {
    file.clear();
    file.seekg(0, ios::beg);
    string line;
    unsigned int line_counter = 0;
    while (true) {
      getline(file, line);
      if (!file) {
        break;
      }
      line_counter++;
      if (string::npos!=line.find("using") && string::npos!=line.find("namespace")) {
        errors++;
        if (mode != 's') {
          cout << line_counter << ": Usage of 'using namespace' is forbidden" << endl;
        }
      }
      if (string::npos!=line.find("printf")) {
        errors++;
        if (mode != 's') {
          cout << line_counter << ": Usage of 'printf' is forbidden" << endl;
          cout << "Use 'std::cout << ' instead" << endl;
        }
      }
      if (string::npos!=line.find("cout") && string::npos!=line.find("\\n")) {
        errors++;
        if (mode != 's') {
          cout << line_counter << ": Usage of '\\n' is forbidden printing to cout" << endl;
          cout << "Use 'std::endl' instead" << endl;
        }
      }
    }
  }
  
  if (mode != 's') {
    cout << "Total errors: " << errors << endl;
  }
  cs_error = errors;
  file.close();
}

void File::checkHCodingStandard(char mode){
  //mode s = silent
  if (type == "unknown") {
    readFileType();
  }
  if (mode == 'l' && type != "h") {
    cout << "Not a valid header file" << endl;
    return;
  }
  ifstream file;
  file.open(path, std::ios::in);
  
  unsigned int errors = 0;
  unsigned int line_counter = 0;
  bool part_public = true;
  string line;
  
  while (true) {
    getline(file, line);
    if (!file) {
      break;
    }
    line_counter++;
    if (string::npos!=line.find("using") && string::npos!=line.find("namespace")) {
      errors++;
      if (mode != 's') {
        cout << line_counter << ": Usage of 'using namespace' is forbidden" << endl;
      }
    }
    if (string::npos!=line.find("public:")) {
      part_public = true;
    }
    if (string::npos!=line.find("private:")) {
      part_public = false;
    }
    if (part_public == true &&
          (
           string::npos!=line.find("int") ||
           string::npos!=line.find("bool") ||
           string::npos!=line.find("char") ||
           string::npos!=line.find("string") ||
           string::npos!=line.find("vector") ||
           string::npos!=line.find("size_t") ||
           string::npos!=line.find("double") ||
           string::npos!=line.find("float") ||
           string::npos!=line.find("long")
          ) && string::npos==line.find("(")
        ) {
      errors++;
      if (mode != 's') {
        cout << line_counter << ": Public variables are forbidden" << endl;
      }
    }
    if (line.size() > 80) {
      errors++;
      if (mode != 's') {
        cout << line_counter << ": More than 80 character" << endl;
      }
    }
  }
  
  if (mode != 's') {
    cout << "Total errors: " << errors << endl;
  }
  cs_error = errors;
  file.close();
}

void File::checkOriginality(string path2){
  
  //getting size
  size_t file1_size = getSize(path);
  size_t file2_size = getSize(path2);
  if (file1_size == file2_size) {
    cout << "Same file size: " << filename << " and " << path2 << endl;
  } else {
    signed long difference = file1_size - file2_size;
    if (difference < 0) {
      difference = difference * -1;
    }
    cout << "Size difference: " << difference << " bytes." << endl;
  }
  
  ifstream file1;
  ifstream file2;
  file1.open(path, std::ios::in);
  file2.open(path2, std::ios::in);
  string current;
  vector<string> file1_lines;
  vector<string> file2_lines;
  
  while (true) {
    file1 >> current;
    if (!file1) {
      break;
    }
    file1_lines.push_back(current);
  }
  
  while (true) {
    file2 >> current;
    if (!file2) {
      break;
    }
    file2_lines.push_back(current);
  }
  
  //search for same variables
  
  vector<string> file1_variables;
  vector<string> file2_variables;
  for (int counter = 0; counter < file1_lines.size(); counter++) {
    if (checkValidVariableType(file1_lines[counter])) {
      current = file1_lines[counter + 1];
      if (checkValidVariable(current)) {
        file1_variables.push_back(current);
      }
    }
  }
  
  for (int counter = 0; counter < file2_lines.size(); counter++) {
    if (checkValidVariableType(file2_lines[counter])) {
      current = file2_lines[counter + 1];
      if (checkValidVariable(current)) {
        file2_variables.push_back(current);
      }
    }
  }
  
  string var1;
  string var2;
  unsigned int match_counter = 0;

  //compare every variable with every variable
  for (int counter = 0; counter < file1_variables.size(); counter++) {
    for (int counter2 = 0; counter2 < file2_variables.size(); counter2++) {
      var1 = file1_variables[counter];
      var2 = file2_variables[counter2];
      if (string::npos!=var1.find(var2) ||
          string::npos!=var2.find(var1) ||
          var1 == var2)
      {
        match_counter++;
        cout << "Possible matching variable: " << var1 << " and " << var2 << endl;
      }
    }
  }
  cout << match_counter << " possible variable matches" << endl;
  var1.clear();
  var1.shrink_to_fit();
  var2.clear();
  var2.shrink_to_fit();
  
  //count cycles
  
  current.clear();
  current.shrink_to_fit();
  unsigned long file1_cycles = 0;
  unsigned long file2_cycles = 0;

  for (int counter = 0; counter < file1_lines.size(); counter++) {
    current = file1_lines[counter];
    if (current == "for" || current == "while") {
      file1_cycles++;
    }
  }
  for (int counter = 0; counter < file2_lines.size(); counter++) {
    current = file2_lines[counter];
    if (current == "for" || current == "while") {
      file2_cycles++;
    }
  }
  
  if (file1_cycles == file2_cycles) {
    cout << "Same ammount of cycle (for+while): " << file1_cycles << endl;
  } else {
    signed long difference = 0;
    difference = file1_cycles - file2_cycles;
    if (difference < 0) {
      difference = difference * -1;
    }
    cout << "Cycle (for/while) difference: " << difference << endl;
  }
  
  //counting if's
  
  unsigned long file1_ifs = 0;
  unsigned long file2_ifs = 0;

  for (int counter = 0; counter < file1_lines.size(); counter++) {
    current = file1_lines[counter];
    if (current == "if") {
      file1_ifs++;
    }
  }
  for (int counter = 0; counter < file2_lines.size(); counter++) {
    current = file2_lines[counter];
    if (current == "if") {
      file2_ifs++;
    }
  }
  
  if (file1_ifs == file2_ifs) {
    cout << "Same ammount of cycle (if): " << file1_cycles << endl;
  } else {
    signed long difference = 0;
    difference = file1_ifs - file2_ifs;
    if (difference < 0) {
      difference = difference * -1;
    }
    cout << "Cycle (if) difference: " << difference << endl;
  }
  
  
  //cleaning up
  file1_lines.clear();
  file1_lines.shrink_to_fit();
  file2_lines.clear();
  file2_lines.shrink_to_fit();
}


