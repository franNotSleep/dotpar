#target: dependencies
#	action

dotpar: to_ts.o dotpar.o 
	gcc to_ts.o dotpar.o -o dotpar -lm

dotpar.o: dotpar.c dotpar.h
	gcc -c dotpar.c

to_ts.o: to_ts.c dotpar.h
	gcc -c to_ts.c

clean:
	rm to_ts.o dotpar.o
