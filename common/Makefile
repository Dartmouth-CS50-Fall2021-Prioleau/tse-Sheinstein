# `common` Makefile 
#
# Salifyanji J Namwila Oct 11, 2021
# 
# Based on Makefile for CS50 library by:
# David Kotz, April 2016, July 2017, April 2019
# Updated by Xia Zhou, July 2016
# Updated by Temi Prioleau, January 2020

L = ../libcs50

PROG = 
OBJS = pagedir.o word.o index.o
LIB = common.a
LLIBS = $L/libcs50.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files

index.o: index.h word.h $L/file.h $L/webpage.h $L/memory.h $L/hashtable.h $L/counters.h 
word.o: word.h
pagedir.o: pagedir.h $L/webpage.h

.PHONY: unittest unit valgrind clean

unittest: index.c unittest.h word.o $L/jhash.o $L/set.o $L/memory.o $L/file.o $L/webpage.o $L/hashtable.o $L/counters.o
	$(CC) $(CFLAGS) -DUNIT_TEST index.c  $L/bag.o word.o $L/jhash.o $L/set.o $L/memory.o $L/file.o $L/webpage.o $L/hashtable.o  $L/counters.o -o unittest
	./unittest

unit: unittest
	./unittest

valgrind: ./unittest
	valgrind ./unittest

clean:
	rm -rf *.dSYM
	rm -f core
	rm -f $(LIB) *~ *.o
	rm -f unittest
	rm -f unittest-loaded-index
	rm -f unittest-saved-index

