# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-lpthread `pkg-config --cflags --libs opencv`


all: control

control: main.cpp
	$(CC) main.cpp $(CFLAGS)
