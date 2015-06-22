This Program is still in BETA! Use with caution!. Read the README for detailed instruction.

# Description:
This program analyzes your C/C++ project, and helps you in many ways. Current features:
 - Code/Comment counter
 - CodingStandard checker
 - Testfile generator
 - Plagiat checker (BETA!!)
 - Multitasking
 - Multi-Core support

## Usage:
The basic usage for the program is: [program] [project_file]
If you are creating a new project, simply input a non-existing file, where the project file should be saved. manually editing the project file is not recommended, however if you are familiar with the structure you can try that. If a project file gets corrupted, CodeCrusher will try to fix it, sometimes it will work, sometimes not.

##Basic Commands:
 - list -> Outputs the files in the current project
 - save -> Saves the project file
 - quit -> Saves the project file and quits the program
 - add / remove -> Manages files
 - process [operators] -> Processes stuff (See below for usage)
 - print [operators] -> Prints stuff (See below for usage)
 - set	[operators] -> Sets file parameters (See below for usage)
 - write -> Generates a test file (See below for usage)

##Managing files
To add a file to your project type "add file", to remove a file type "remove file". To see your files use the "list" command.

##The print command
The print command prints various stats of a file. The most basic operator of the print command is "all". This will give you every stat of every file in your project. If you want to see the stats of your whole project you can use the "db" operator.
 - print [operator]

To get the stats of a specific file you use:
 - print [filename]

The print command is also useful when you want to use the built-in analysis engines:
 - print [filename] [engine]

There are 2 analysis engines in CodeCrusher, here is a list of the operators:
 - cs -> Coding Standard Checker
 - pl -> Plagiat checker

##The process command
The files won't be processed when you add them to the project, you have to use the process command:
 - process [operator]
 - process [filename] [operator]

To process the entire project, use the "db" operator. To process a singe file, type in the filename and use no operators. If your project is big, it could take a while to process every file. CodeCrusher supports multitasking, the processing function runs on a separate thread. 

##The set command
Using the set command, you can set various file flags:
 - set [filename] [flag] [value]

Here is a list of flags:
 - w -> Writable (Default: false)

Set can also change database settings:
 - set db [attribute]

Here is a list of attrivutes:
 - name -> Database name

##The write command
To generate a test file, first, you have to add it to your project, use the add command. If you want to create a new file use the add command, like if the file would exist. Then, you must use the set command to set the file to writeable. After that, use the write command to generate your test file.
 - write [filename]
 
When writing a test file, the writer function runs on a different thread, so you can still use the program.

##Other commands:
To disable the autosave feature, type "disable autosave".