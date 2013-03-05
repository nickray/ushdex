CXX := g++
HEADERS := meta_rw.h red_rw.h session.h topN_rw.h trade_rw.h types.h util.h
LIBS := -lpthread -lrt
BOOST_LOC := /opt/boost_1_53_0
EIGEN_LOC = /opt/eigen-3.1.2

.PHONY: all clean dev rel get_eigen

dev: CXXFLAGS := -std=c++0x -Wall -Werror -g -Wfatal-errors -I$(BOOST_LOC)
dev: all

# Some say that -Os is faster than -Os due to cache line
# optimizations or whatever, but experimentally -O3 wins here
rel: CXXFLAGS := -std=c++0x -O3 -I$(BOOST_LOC) -I$(EIGEN_LOC) -DUSE_EIGEN
rel: all
	#find .  -maxdepth 1 -executable -type f -exec strip {} \;
	#find .  -maxdepth 1 -executable -type f -exec /opt/upx --brute {} \;

all: $(HEADERS) clean_shm.o write_test.o read_test.o minimal_writer_example.o minimal_reader_example.o lookup.o list.o rw_test.o
	$(CXX) -o clean_shm clean_shm.o $(LIBS)
	$(CXX) -o minimal_writer_example minimal_writer_example.o $(LIBS)
	$(CXX) -o minimal_reader_example minimal_reader_example.o $(LIBS)
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
	rm -f clean_shm list lookup minimal_writer_example minimal_reader_example write_test read_test  rw_test

get_boost:
	wget --quiet http://downloads.sourceforge.net/project/boost/boost/1.53.0/boost_1_53_0.tar.bz2
	tar xf boost_1_53_0.tar.bz2
	-sudo rm -rf ${BOOST_LOC}
	-sudo mv boost_1_53_0 /opt
	rm -rf boost_1_53_0*

get_eigen:
	wget --quiet http://bitbucket.org/eigen/eigen/get/3.1.2.tar.bz2
	tar xf 3.1.2.tar.bz2
	-sudo rm -rf ${EIGEN_LOC}
	-sudo mv eigen-eigen-5097c01bcdc4 ${EIGEN_LOC}
	rm -rf 3.1.2*

