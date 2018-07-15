# This Makefile is used to create an executable on a Unix-like system.
# It is not used for builing on the Arduino.
# It needs https://github.com/bengtmartensson/Infrared4Arduino to be installed.
# see the INCLUDES line below.
# (This does not apply to the Arduino build of course.)

NAME=DecodeIR
OBJS = $(NAME).o main.o

# Include the following if using the class wrapper
INCLUDES=-I../Infrared4Arduino/src/
DEFINES=-DUSECLASS
OBJS += DecodeIRClass.o

VPATH=src
CC=g++
#WARN=-Wall -Wextra
OPT=-O2

all: test

clean:
	rm -f decoder $(OBJS)

%.o: %.cpp
	$(CC) $(DEFINES) $(INCLUDES) -g $(WARN) $(OPT) -c $<

decoder: $(OBJS)
	g++ -g $(WARN) $(OPT) -o $@ $(OBJS)

test: decoder
	./decoder 0000 006C 0022 0002 015B 00AD 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 06A4 015B 0057 0016 0E6C
