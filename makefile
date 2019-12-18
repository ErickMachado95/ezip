ezip : ezip.o compression.o
	gcc -o ezip ezip.o compression.o


compression.o: compression.c 
	gcc -c compression.c

ezip.o: ezip.c
	gcc -c ezip.c

remove_o: 
	rm ezip.o compression.o

clean: 
	rm ezip
