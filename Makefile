#target: dependencies
#	action

dotpar: to_ts.o dotpar.o
	gcc to_ts.o dotpar.o -o dotpar -lm

dotpar.o: dotpar.c
	gcc -c dotpar.c

to_ts.o: to_ts.c
	gcc -c to_ts.c
