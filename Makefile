OUTPUT = codecrusher
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

%.o: %.cpp
	$(CXX) $(OBJFLAGS) $<

.PHONY: clean
clean:
	@rm $(OBJ) codecrusher