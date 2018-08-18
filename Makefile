CC=gcc
CFLAG="-O3 -flto -Wall"

all:ohas ohvm
ohas:nas.o
	${CC} ${CFLAG} $^ -o $@
ohvm:nvm.o
	${CC} ${CFLAG} $^ -o $@
%.o: %.c
	${CC} ${CFLAG} -c $< -o $@
clean:
	rm -f ohas ohvm *.o
