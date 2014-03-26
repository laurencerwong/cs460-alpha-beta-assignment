CC=g++
CFLAGS=-g -Wall

all: 
	$(CC) $(CFLAGS) main.cpp node.cpp -o main

