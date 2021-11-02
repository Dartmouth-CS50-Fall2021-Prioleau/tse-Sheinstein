# Makefile for 'querier'
#
# Salifyaji J Namwila Oct 24 2021
# 
# Based on Makefile for bags module by:
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020

L = ../libcs50

PROG = querier
OBJS = querier.o ../common/pagedir.o ../common/index.o ../common/word.o
LIBS =
LLIBS = $L/libcs50.a ../common/common.a

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=gnu11 -ggdb $(TESTING) -I$L -I../common
CC = gcc
MAKE = make

all: querier fuzzquery

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

fuzzquery: fuzzquery.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test qunittest unit valgrind clean

test:
	bash -v testing.sh &> testing.out


qunittest: querier.c  qunittest.h ../common/pagedir.o ../common/index.o ../common/word.o $L/jhash.o $L/set.o $L/memory.o $L/file.o $L/webpage.o $L/hashtable.o $L/counters.o
	$(CC) $(CFLAGS) -DUNIT_TEST querier.c  $L/bag.o ../common/pagedir.o ../common/index.o ../common/word.o $L/jhash.o $L/set.o $L/memory.o $L/file.o $L/webpage.o $L/hashtable.o  $L/counters.o -o qunittest
	./qunittest

# uncommnet here to run valgrind on  querier unittest, qunittest
#valgrind: qunittest
#valgrind ./qunittest

valgrind: $(PROG) 
	valgrind ./$(PROG) 

clean:
	rm -rf *~ *.o *.dSYM
	rm -f querier
	rm -f stocks
	rm -f core
	rm -f fuzzquery 
	rm -f querier
	rm -f qunittest



