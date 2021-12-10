all: mask

mask: main.o passwordmask.o scaletoscale.o
	gcc main.o passwordmask.o scaletoscale.o -o mask

main.o: main.c
	gcc -c main.c

passwordmask.o: passwordmask.c
	gcc -c passwordmask.c

scaletoscale.o: scaletoscale.c
	gcc -c scaletoscale.c

clean:
	rm -rf *.o mask
