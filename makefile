CC  = g++
CXX = g++

CFLAGS   = -g -Wall
CXXFLAGS = -g -Wall -std=c++14 -fno-elide-constructors

executables = helloppm

.PHONY: default
default: $(executables)

helloppm: helloppm.o


.PHONY: clean
clean:
	rm -f *~ a.out core *.o $(executables)

.PHONY: all
all: clean default