CC = g++
CFLAG = -g -Wall -std=c++11
GTEST_LL = -I /usr/local/opt/gtest/include/ -l gtest -l gtest_main -pthread

all: test

test: gtest_heap.cpp heap.h
	$(CC) $(CFLAG) $^ $(GTEST_LL) -o $@

.PHONY:

clean:
	rm -rf test