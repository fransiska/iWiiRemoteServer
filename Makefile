#!/usr/bin/make

CFLAGS= -c -Wall

%.o: %.cpp
	gcc $(CFLAGSS) $<
