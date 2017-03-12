CC=gcc
CFLAGS=-O3
LFLAGS=-lwiringPi
LD=gcc

all: build1 build2 link

build1: hd44780.c
	$(CC) $(CFLAGS) -o hd44780.o -c hd44780.c

build2: main.c
	$(CC) $(CFLAGS) -o main.o -c main.c

link: hd44780.o main.o
	$(LD) $(LFLAGS) -o hd44780_test hd44780.o main.o
