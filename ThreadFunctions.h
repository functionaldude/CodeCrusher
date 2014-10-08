//
//  ThreadFunctions.h
//  CodeCrusher
//
//  Created by Zoltán Sasvári on 06.05.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#ifndef __CodeCrusher__ThreadFunctions__
#define __CodeCrusher__ThreadFunctions__

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "Database.h"

void MULTIprocessDB(Database& activedb, std::mutex& mtx_cout);
void MULTIwriteFile(Database& activedb, std::string filename, std::vector<std::string> lines, unsigned long line_multiplier, char mode, std::mutex& mtx_cout);
void MULTIprocessFile(Database& activedb, std::string& filename, std::mutex& mtx_cout);
void MULTIautosave(Database& activedb, std::mutex& save, bool& state);

#endif /* defined(__CodeCrusher__ThreadFunctions__) */
