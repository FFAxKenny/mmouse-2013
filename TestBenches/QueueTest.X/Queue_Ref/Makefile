# Makefile for executable queuetest

# *****************************************************
# Parameters to control Makefile operation

CC = gcc
CFLAGS = -ansi -pedantic -Wall -g

# ****************************************************
# Entries to bring the executable up to date

queuetest: queuetest.o queue.o
	$(CC) $(CFLAGS) -o queuetest queuetest.o queue.o

queuetest.o: queuetest.c queue.h
	$(CC) $(CFLAGS) -c queuetest.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c
