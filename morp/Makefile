CFLAGS=-O3
SRC = src
BIN = bin
CC = clang

all: mincost longest
	
mincost:
	$(CC) $(CFLAGS) -o $(BIN)/mincost $(SRC)/mincost.c $(SRC)/dic.c $(SRC)/chars.c

longest:
	$(CC) $(CFLAGS) -o $(BIN)/longest $(SRC)/mincost.c $(SRC)/dic.c $(SRC)/chars.c
