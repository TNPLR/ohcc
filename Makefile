.PHONY: clean
CC=gcc
CFLAG=-O2 -Wall

all:ohvm ohas
ohvm:vm.o libc.o
	${CC} ${CFLAG} $^ -o $@
ohas:as.o
	${CC} ${CFLAG} $^ -o $@
ohcc:cc.o
	${CC} ${CFLAG} $^ -o $@
nvm:nvm.o
	${CC} ${CFLAG} $^ -o $@
%.o: %.c
	${CC} ${CFLAG} -c $< -o $@
clean:
	rm -f ohvm ohas nvm *.o
