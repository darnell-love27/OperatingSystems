# list/Makefile
#
# Makefile for list implementation and test file.
#
# <Darnell Love>
# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = list.c main.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = list

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)

.PHONY: all clean


