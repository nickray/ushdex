LIBS := -lrt -lpthread

.PHONY: all

all: *.o
    g++ -o create_shm create_shm.o $(LIBS)
    g++ -o write_test write_test.o $(LIBS)
    g++ -o read_test read_test.o $(LIBS)
    g++ -o lookup lookup.o $(LIBS)

%.o: %.cc
    g++ -std=c++0x -Wall -g -Os -c $(input) -o $(output)

.PHONY: clean

clean:
	rm -f *.o core
	rm -f create_shm write_test read_test
