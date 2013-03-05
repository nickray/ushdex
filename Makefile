CXX := g++
BOOST_LOC := /opt/boost_1_53_0
EIGEN_LOC := /opt/eigen-3.1.2
CXXFLAGS := -std=c++0x -Wall -I$(BOOST_LOC)

SOURCES := $(wildcard *.cc)
HEADERS := $(wildcard *.h)
OBJECTS := $(SOURCES:.cc=.o)
BINARIES := $(OBJECTS:.o=)

LIBS := -lpthread -lrt

.PHONY: all clean dev rel get_eigen

dev: CXXFLAGS += -Werror -Wfatal-errors -g
dev: all

# Some say that -Os is faster than -Os due to cache line
# optimizations or whatever, but experimentally -O3 wins here
rel: CXXFLAGS += -O3 -I$(EIGEN_LOC) -DUSE_EIGEN
rel: all
	strip $(BINARIES)

all: $(HEADERS) $(SOURCES) $(OBJECTS) $(BINARIES)

%_rw.h: %.vars
	python generate.py $(<:.vars=)

%.o: %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%: %.o
	$(CXX) -o $@ $< $(LIBS)

clean:
	rm -f $(OBJECTS) core
	rm -f $(BINARIES)

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

