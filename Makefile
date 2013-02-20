
# This is Makefile assumes Makepp

CXX := g++-4.7

# Some say that -Os is faster than -Os due to cache line
# optimizations or whatever, but it seems -O3 is fater here
CXXFLAGS := -std=c++11 -Wall -Werror -g -Wfatal-errors -O3 

LIBS := -lrt -lpthread

.PHONY: all

all: *.o
	$(CXX) -o clean_shm clean_shm.o $(LIBS)
	$(CXX) -o write_test write_test.o $(LIBS)
	$(CXX) -o read_test read_test.o $(LIBS)
	$(CXX) -o lookup lookup.o $(LIBS)
	$(CXX) -o list list.o $(LIBS)
	$(CXX) -o test_diamond test_diamond.o $(LIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $(input) -o $(output)

.PHONY: clean

clean:
	rm -f *.o core
	rm -f clean_shm write_test read_test list lookup test_diamond

