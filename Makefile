LIBS := -lrt -lpthread

.PHONY: all

all: *.o
    g++ -o create_shm create_shm.o $(LIBS)
    #g++ -o write_shm write_shm.cc -lrt -lpthread
    #g++ -o read_shm read_shm.cc -lrt -lpthread

%.o: %.cc
    g++ -std=c++0x -Wall -g -c $(input) -o $(output)

