
# This is Makefile assumes Makepp

CXX := /opt/g++-4.7

LIBS := -lpthread -lrt

.PHONY: all clean dev rel

dev: CXXFLAGS := -std=c++11 -Wall -Werror -g -Wfatal-errors -I/opt/boost_1_53_0
dev: all

# Some say that -Os is faster than -Os due to cache line
# optimizations or whatever, but experimentally -O3 wins here
rel: CXXFLAGS := -std=c++11 -O3 -I/opt/boost_1_53_0
rel: all
	find .  -maxdepth 1 -executable -type f -exec strip {} \;
	#find .  -maxdepth 1 -executable -type f -exec /opt/upx --brute {} \;

all: trade_rw.h red_rw.h clean_shm.o write_test.o read_test.o lookup.o list.o test_diamond.o
	$(CXX) -o clean_shm clean_shm.o $(LIBS)
	$(CXX) -o write_test write_test.o $(LIBS)
	$(CXX) -o read_test read_test.o $(LIBS)
	$(CXX) -o lookup lookup.o $(LIBS)
	$(CXX) -o list list.o $(LIBS)
	$(CXX) -o test_diamond test_diamond.o $(LIBS)

trade_rw.h: trade.vars
	python generate.py trade

red_rw.h: red.vars
	python generate.py red

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o core
	find .  -maxdepth 1 -executable -type f -exec rm {} \;

