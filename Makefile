CFLAGS=-std=c11 -g -static

9cc.o:9cc.c

test:9cc.o
	./test.sh

clean:
	rm -f 9cc.o *.o *~ tmp.exe*

.PHONY: test clean


