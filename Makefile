OUTPUT = codecrusher
CXX = g++
CXXFLAGS = -O3 -std=c++11
OBJFLAGS = -O3 -std=c++11 -c
SRC =

SRC += main.cpp
SRC += Database.cpp
SRC += Algorithms.cpp
SRC += File.cpp
SRC += ThreadFunctions.cpp
SRC += Dialogs.cpp

OBJ =

OBJ += main.o
OBJ += Database.o
OBJ += Algorithms.o
OBJ += File.o
OBJ += ThreadFunctions.o
OBJ += Dialogs.o

all: $(OBJ)
	@echo "Linking object files"
	$(CXX) $(CXXFLAGS) -o ./$(OUTPUT) $(OBJ)
	@rm $(OBJ)

main.o:
	@echo "Building main.o"
	$(CXX) $(OBJFLAGS) main.cpp
Database.o:
	@echo "Building Database.o"
	$(CXX) $(OBJFLAGS) Database.cpp
Algorithms.o:
	@echo "Building Algorithms.o"
	$(CXX) $(OBJFLAGS) Algorithms.cpp
File.o:
	@echo "Building File.o"
	$(CXX) $(OBJFLAGS) File.cpp
ThreadFunctions.o:
	@echo "Building ThreadFunctions.o"
	$(CXX) $(OBJFLAGS) ThreadFunctions.cpp
Dialogs.o:
	@echo "Building Dialogs.o"
	$(CXX) $(OBJFLAGS) Dialogs.cpp