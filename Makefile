# Compiler definitions
CC = gcc          # C compiler
CXX = g++         # C++ compiler

# Compiler flags
CFLAGS = -Iinclude -MMD -Wall -ggdb3		 	# Flags for C files: include path and dependency generation
CXXFLAGS = -Iinclude -MMD -Wall -ggdb3		# Flags for C++ files: include path and dependency generation

# Source files
SRCS = main.c src/DoubleTree.cpp src/graph_rand.c src/graph_tests.c src/graph.c

# Object files (derived from source files)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRCS)))

# Dependency files (derived from object files)
DEPS = $(OBJS:.o=.d)

# Default target: the executable
main: $(OBJS)
	$(CXX) $(OBJS) -o main

# Rule to compile C files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile C++ files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files if they exist
-include $(DEPS)

# Phony target for cleaning up
.PHONY: clean

# Clean target to remove generated files
clean:
	rm -f $(OBJS) $(DEPS) main