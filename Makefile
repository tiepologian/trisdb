CC=gcc
CXX=g++
RM=rm -f
SRCS=src/*.cpp src/*.cc
OBJS=$(subst .cc,.o,$(SRCS))
CPPFLAGS=-std=c++11 -O3 -DUSE_BOOST_KARMA

all:
	g++ $(CPPFLAGS) $(SRCS) -o dist/trisdb -lreadline -lprotobuf -pthread -lboost_system

clean:
	$(RM) dist/trisdb
