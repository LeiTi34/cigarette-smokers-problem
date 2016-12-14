#Autor: Alex Leidwein
CC = /usr/bin/gcc
SRC = main.c
BIN = $(SRC:%.c=%.out)

all: main
		
main: main.c
	gcc main.c -o main.out -lpthread 

.PHONY: clean
clean:
	rm -rf $(BIN)