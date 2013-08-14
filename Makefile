CFLAGS := -Wall -g -I

all: shell


shell: shell.c strfunc.o
	gcc shell.c strfunc.o -o clam -lreadline

strfunc.o:
	gcc -c strfunc.c -o strfunc.o

clean:
	rm -f clam *.o
