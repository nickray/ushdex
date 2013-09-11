CXX := g++
BOOST_LOC := /opt/boost_1_54_0
EIGEN_LOC := /opt/eigen-3.1.2
EIGEN_ARCH_LOC := /usr/include/eigen3
CXXFLAGS := -std=c++11 -Wall -MMD -I$(BOOST_LOC)
#CPLUS_INCLUDE_PATH=$(EIGEN_ARCH_LOC):$(EIGEN_LOC):$(BOOST_LOC)

SOURCES := $(wildcard *.cc)
OBJECTS := $(SOURCES:.cc=.o)
DEPENDENCIES :=  $(OBJECTS:.o=.d)
BINARIES := $(OBJECTS:.o=)
LIBS := -lpthread -lrt

.PHONY: all clean dev rel get_boost get_eigen

dev: CXXFLAGS += -Werror -Wfatal-errors -g
dev: all

# in this case -O3 is faster than -Os
rel: CXXFLAGS += -O3#-I$(EIGEN_LOC) -I$(EIGEN_ARCH_LOC) -DUSE_EIGEN
rel: all
	strip $(BINARIES)

all: $(SOURCES) $(OBJECTS) $(BINARIES)

-include $(DEPENDENCIES)

# handy suffix rules
%_rw.h: %.vars generate.py
	python generate.py $(<:.vars=)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%: %.o
	$(CXX) -o $@ $< $(LIBS)

clean:
	rm -f core *.d *.o $(BINARIES)

get_boost:
	wget --quiet http://downloads.sourceforge.net/project/boost/boost/1.54.0/boost_1_54_0.tar.bz2
	tar xf boost_1_54_0.tar.bz2
	-sudo rm -rf ${BOOST_LOC}
	-sudo mv boost_1_54_0 /opt
	rm -rf boost_1_54_0*

get_eigen:
	wget --quiet http://bitbucket.org/eigen/eigen/get/3.1.2.tar.bz2
	tar xf 3.1.2.tar.bz2
	-sudo rm -rf ${EIGEN_LOC}
	-sudo mv eigen-eigen-5097c01bcdc4 ${EIGEN_LOC}
	rm -rf 3.1.2*

