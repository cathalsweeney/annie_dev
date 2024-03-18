# Makefile for compiling code with ROOT and Minuit

# Compiler
CC = g++

# ROOT configuration
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS := $(shell root-config --libs)

# Include directories for ROOT
ROOTINCS = $(shell root-config --cflags)

# Minuit flags
MINUITLIBS := -lMinuit

# Compiler flags
CFLAGS = -Wall -O2

# Source files
SRCS = RunFit.cpp KnobFitter.cpp

# Header files
HEADERS = KnobFitter.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = RunFit

# Main target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(ROOTINCS) $(ROOTLIBS) $(MINUITLIBS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(ROOTCFLAGS) $(ROOTINCS) -c -o $@ $<


# Clean rule
clean:
	rm -f $(OBJS) $(EXEC)
