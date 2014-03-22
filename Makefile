CC=g++
CFLAGS=-g -Wall

all: main.o node.o
	$(CC) $(CFLAGS) main.cpp node.cpp -o main

