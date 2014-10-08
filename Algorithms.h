//
//  Algorithms.h
//  CodeCrusher
//
//  Created by Zoltán Sasvári on 13.05.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#ifndef __CodeCrusher__Algorithms__
#define __CodeCrusher__Algorithms__

#include <iostream>
#include <regex>
#include <vector>
#include <mutex>

bool checkMutexStatus(std::mutex* input);
bool checkValidVariable(std::string input);
bool checkValidVariableType(std::string input);

#endif /* defined(__CodeCrusher__Algorithms__) */
