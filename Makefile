CC=gcc
CXX=g++
RM=rm -f
SRCS=src/*.cpp
OBJS=$(subst .cc,.o,$(SRCS))
CPPFLAGS=-std=c++11 -O3

all:
	g++ $(CPPFLAGS) $(SRCS) -o dist/trisdb

clean:
	$(RM) dist/trisdb
