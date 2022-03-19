9cc.o:9cc.c
	gcc 9cc.c -o 9cc.o

test:9cc.o
	./test.sh

clean:
	rm -f 9cc.o *.o *~ tmp.exe*

.PHONY: test clean


