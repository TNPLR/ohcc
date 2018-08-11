.PHONY: clean
CC=gcc
CFLAG=-O2 -Wall

all:ohvm ohas
ohvm:
	${CC} ${CFLAG} vm.c -o ohvm
ohas:
	${CC} ${CFLAG} as.c -o ohas
clean:
	rm -f ohvm ohas
