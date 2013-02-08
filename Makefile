CXXFLAGS := -std=c++0x -Wall -Werror -g -Os
LIBS := -lrt -lpthread

.PHONY: all

all: *.o
	g++ -o clean_shm clean_shm.o $(LIBS)
	g++ -o write_test write_test.o $(LIBS)
	g++ -o read_test read_test.o $(LIBS)
	g++ -o lookup lookup.o $(LIBS)
	g++ -o list list.o $(LIBS)

%.o: %.cc
	g++ $(CXXFLAGS) -c $(input) -o $(output)

.PHONY: clean

clean:
	rm -f *.o core
	rm -f clean_shm write_test read_test list lookup
