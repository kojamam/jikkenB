CFLAGS=-O3 -ansi
#CFLAGS=-ansi -g
BIN = ../bin
CC = gcc

all: ${BIN}/jedic ${BIN}/mkjedic ${BIN}/chseg ${BIN}/chnum ${BIN}/dechex ${BIN}/readdic

${BIN}/jedic: jedic.c
	${CC} ${CFLAGS} -o ${BIN}/jedic jedic.c

${BIN}/mkjedic: mkjedic.c
	${CC} ${CFLAGS} -o ${BIN}/mkjedic mkjedic.c

${BIN}/chseg: chseg.c
	${CC} ${CFLAGS} -o ${BIN}/chseg chseg.c

${BIN}/chnum: chnum.c
	${CC} ${CFLAGS} -o ${BIN}/chnum chnum.c

${BIN}/dechex: dechex.c
	${CC} ${CFLAGS} -o ${BIN}/dechex dechex.c

${BIN}/readdic: readdic.c
	${CC} ${CFLAGS} -o ${BIN}/readdic readdic.c
