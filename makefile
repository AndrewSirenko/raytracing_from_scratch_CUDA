CC  = g++
CXX = g++

CFLAGS   = -g -Wall
CXXFLAGS = -g -Wall -std=c++14 -fno-elide-constructors

executables = main

.PHONY: default
default: $(executables)

main: main.o 


.PHONY: clean
clean:
	rm -f *~ a.out core *.o $(executables)

.PHONY: all
all: clean default