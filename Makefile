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

install:
	install -D dist/trisdb /usr/local/bin/trisdb
	install -D utils/trisdb-init.conf /etc/init/trisdb.conf

uninstall:
	$(RM) /usr/local/bin/trisdb
	$(RM) /etc/init/trisdb.conf
