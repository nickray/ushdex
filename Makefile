
# This is Makefile assumes Makepp

CXX := g++-4.7

CXXFLAGS := -std=c++11 -Wall -Werror -g -Os
CXXFLAGS := -std=c++11 -Wall -g -Os
LIBS := -lrt -lpthread

.PHONY: all

all: *.o
	$(CXX) -o clean_shm clean_shm.o $(LIBS)
	$(CXX) -o write_test write_test.o $(LIBS)
	$(CXX) -o read_test read_test.o $(LIBS)
	$(CXX) -o lookup lookup.o $(LIBS)
	$(CXX) -o list list.o $(LIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $(input) -o $(output)

.PHONY: clean

clean:
	rm -f *.o core
	rm -f clean_shm write_test read_test list lookup

