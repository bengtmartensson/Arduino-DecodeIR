# This Makefile is used to create an executable on a Unix-like system.
# It is not used for builing on the Arduino.

NAME=DecodeIR
CC=g++
#WARN=-Wall -Wextra
OPT=-O2

all: test

VPATH=src
OBJS = $(NAME).o main.o	

clean:
	rm -f decoder $(OBJS)

%.o: %.cpp
	$(CC) -g $(WARN) $(OPT) -c $<

decoder: $(OBJS)
	g++ -g $(WARN) $(OPT) -o $@ $(OBJS)

test: decoder
	./decoder 0000 006C 0022 0002 015B 00AD 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 06A4 015B 0057 0016 0E6C
