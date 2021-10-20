# Makefile for 'indexer' module
#
# CS50 Fall 2021 Lab 5 -Indexer
# Author: Salifyanji J Namwila
#
# Date: 0ctober 19 2021
#
# Based on Makefile for bags module by:
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020

L = ../libcs50

PROG = 
OBJS = ../common/pagedir.o ../common/index.o ../common/word.o
LIBS =
LLIBS = $L/libcs50.a ../common/common.a

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I../common
CC = gcc
MAKE = make

all: indexer indextest

indexer: indexer.o $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest: indextest.o $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test clean

clean:
	rm -f *~ *.o *.dSYM
	rm -f indexer indextest
	rm -f stocks
	rm -f core
test:
	bash testing.sh