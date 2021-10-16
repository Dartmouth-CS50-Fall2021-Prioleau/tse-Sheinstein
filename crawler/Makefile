# Makefile for 'crawler'
#
# Salifyanji J Namwila, Oct 11, 2020
# 
# Based on Makefile for bags module by:
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020
# Makefile for 'crawler' module
#
# Irene Lam, July 27, 2020
# CS50, Summer 2020

PROG = crawler
OBJS = crawler.o 
# LIBS = ../libcs50/memory.o ../libcs50/bag.o ../libcs50/hashtable.o ../libcs50/webpage.o ../libcs50/set.o ../libcs50/jhash.o ../libcs50/file.o
LIBS = ../libcs50/libcs50.a ../common/common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50
CC = gcc
MAKE = make
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test valgrind clean

test:

	bash testing.sh &> testing.out

valgrind: $(PROG) 
# 	valgrind ./$(PROG) 

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
