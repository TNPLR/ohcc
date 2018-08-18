.PHONY: clean
CC=gcc
CFLAG=-O3 -Wall -flto

all:ohvm ohas ohcc
ohvm:vm.o libc.o
	${CC} ${CFLAG} $^ -o $@
ohas:as.o
	${CC} ${CFLAG} $^ -o $@
ohcc:cc.o
	${CC} ${CFLAG} $^ -o $@
%.o: %.c
	${CC} ${CFLAG} -c $< -o $@
clean:
	rm -f ohvm ohas nvm nas *.o
