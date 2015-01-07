CLFAGS=-Wall -g -std=c99

extract: extract.o xld.o fs.o
	gcc -o extract extract.o xld.o fs.o
extract.o: 
	gcc $(CFLAGS) -c src/extract.c
xld.o:
	gcc $(CFLAGS) -c src/xld.c
fs.o:
	gcc $(CFLAGS) -c src/fs.c
clean:
	rm extract *.o

