# Payal Mistry
# CS 302 - Programmig Methodologies and Software Implementation
# Assignment 3 - Cruise If You Can
# May 17 2026
# Filename: Makefile
#
# This Makefile builds the "Cruise If You Can" driving
# game. The build is split into separate object files
# so any source file that changes only triggers a
# recompile of that file and the final link, instead
# of rebuilding the whole project every time. The 
# default target is the executable named "cruise".
# Run "make" to build, "make clean" to remove build
# products, and "make run" to build and then run the
# program in one step.

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall

# Executable name - target
TARGET = cruise

# Object files
OBJS = obstacle_mistry.o hazards_mistry.o bst_mistry.o main_mistry.o

# Defult target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

obstacle_mistry.o: obstacle_mistry.cpp obstacle_mistry.h
	$(CXX) $(CXXFLAGS) -c obstacle_mistry.cpp

hazards_mistry.o: hazards_mistry.cpp hazards_mistry.h obstacle_mistry.h
	$(CXX) $(CXXFLAGS) -c hazards_mistry.cpp

bst_mistry.o: bst_mistry.cpp bst_mistry.h obstacle_mistry.h hazards_mistry.h
	$(CXX) $(CXXFLAGS) -c bst_mistry.cpp

main_mistry.o: main_mistry.cpp obstacle_mistry.h hazards_mistry.h bst_mistry.h
	$(CXX) $(CXXFLAGS) -c main_mistry.cpp

# build and run
run: $(TARGET) 
	./$(TARGET)

# clean
clean: 
	rm -f $(OBJS) $(TARGET)
