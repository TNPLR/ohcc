CC=gcc
CFLAG=-O2

all: ohvm
ohvm:
	${CC} ${CFLAG} vm.c -o ohvm
clean:
	rm -f ohvm
