CLFAGS=-Wall -g

extract: extract.o xld.o
	gcc -o extract extract.o xld.o
extract.o: 
	gcc $(CFLAGS) -c extract.c
xld.o:
	gcc $(CFLAGS) -c xld.c

clean:
	rm extract *.o

