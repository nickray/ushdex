
CXX := g++

HEADERS := meta_rw.h red_rw.h session.h topN_rw.h trade_rw.h types.h util.h

LIBS := -lpthread -lrt

.PHONY: all clean dev rel

dev: CXXFLAGS := -std=c++0x -Wall -Werror -g -Wfatal-errors -I/opt/boost_1_53_0
dev: all

# Some say that -Os is faster than -Os due to cache line
# optimizations or whatever, but experimentally -O3 wins here
rel: CXXFLAGS := -std=c++0x -O3 -I/opt/boost_1_53_0
rel: all
	#find .  -maxdepth 1 -executable -type f -exec strip {} \;
	#find .  -maxdepth 1 -executable -type f -exec /opt/upx --brute {} \;

all: $(HEADERS) clean_shm.o write_test.o read_test.o lookup.o list.o rw_test.o
	$(CXX) -o clean_shm clean_shm.o $(LIBS)
	$(CXX) -o write_test write_test.o $(LIBS)
	$(CXX) -o read_test read_test.o $(LIBS)
	$(CXX) -o lookup lookup.o $(LIBS)
	$(CXX) -o list list.o $(LIBS)
	$(CXX) -o rw_test rw_test.o $(LIBS)

trade_rw.h: trade.vars
	python generate.py trade

red_rw.h: red.vars
	python generate.py red

%.o: %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o core
	#find .  -maxdepth 1 -executable -type f -exec rm {} \;
	rm -f clean_shm write_test read_test lookup list rw_test

