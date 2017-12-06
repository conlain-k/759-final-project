# Warnings
WFLAGS := -Wall -Wextra -Wno-sign-conversion -Wsign-compare
RPATHFLAGS := -Wl,-rpath=/home/conlain/programs/759-final-project
TOBJFLAGS := -Ltinyobjloader -l tinyobjloader_double

# Optimization and architecture
OPT  := -O3 -mavx -ftree-vectorize -msse2

# Language standard
CCSTD	:= -std=c99
CXXSTD	:= -std=c++14

FLAGS	:=-fopenmp

# Linker options
LDFLAGS := -fopenmp

# Names of executables to create
EXEC := tester

.DEFAULT_GOAL := all

all : Makefile $(EXEC)
tester: solver.cpp
	@ echo Compiling $<...
	 $(CXX) $(CXXSTD) $(WFLAGS) $(FLAGS) $(RPATHFLAGS) $(TOBJFLAGS) $(CXXFLAGS) $< -o $@
	 cp tinyobjloader/libtinyobjloader_double.so.1 .

.PHONY: clean
clean:
	@ rm -f *.o $(EXEC) *.so libtinyobjloader_double.so.1
