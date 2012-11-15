CXXFLAGS += -std=c++11 -stdlib=libc++ -ggdb
LDFLAGS += -lc++

all: main

main: main.o apftest.o

main.o: main.cc sort.h vector.h
apftest.o: apftest.cc

.PHONY: clean
clean:
	-rm -rf *.o main
