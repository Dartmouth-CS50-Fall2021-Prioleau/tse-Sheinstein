# Makefile for 'crawler'
#
# Salifyanji J Namwila, Oct 11, 2020
# 
# Based on Makefile for bags module by:
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020

P = ../libcs50

PROG = crawler
OBJS = crawler.o ../common/pagedir.o
LIBS =
LLIBS = $P/libcs50.a ../common/common.a

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$P -I../common
CC = gcc
MAKE = make

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 


.PHONY: test clean

test:
	bash testing.sh
clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f core


