#
# CS61064 - High Perfomance Parallel Programming
# OpenMP/MPI - Assignment 2
# Twin Prime Numbers
# 
# Author: Utkarsh Patel (18EC35034)
#
# Makefile for building the source
#

.PHONY: all clean

all: serial parallel

serial.o: serial.c
	gcc -g -Wall -c $^ -o $@

serial: serial.o
	gcc -g -Wall $^ -o $@

parallel.o: parallel.c
	mpicxx -g -Wall -c $^ -o $@

parallel: parallel.o
	mpicxx -g -Wall $^ -o $@

clean:
	rm -f serial serial.o parallel parallel.o
