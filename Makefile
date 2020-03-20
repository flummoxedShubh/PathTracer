CC=g++
CFLAGS=-Wall

all:program

program:main.cpp
	$(CC) $(CFLAGS) main.cpp -o program

