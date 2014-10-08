//
//  Dialogs.h
//  LineCounter2
//
//  Created by Zoltán Sasvári on 28.04.14.
//  Copyright (c) 2014 G-TEAM. All rights reserved.
//

#ifndef __LineCounter2__Dialogs__
#define __LineCounter2__Dialogs__

#include <iostream>
#include <vector>

void printHelp();
void printSeparator();
std::vector<std::string> splitArgs(std::string s);
void flushInput();

#endif /* defined(__LineCounter2__Dialogs__) */
