# Makefile для создания статической библиотеки libtag_termios_mu.a

CC = gcc
AR = ar
CFLAGS = -Wall -O2
TARGET = libtag_termios_mu.a
SRC = tag_termios_mu.c
OBJ = tag_termios_mu.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(AR) rcs $@ $^

%.o: %.c tag_termios_mu.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
