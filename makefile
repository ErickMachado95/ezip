ezip : ezip.o compression.o decompression.o
	gcc -o ezip ezip.o compression.o decompression.o
	rm ezip.o compression.o decompression.o


compression.o: compression.c 
	gcc -c compression.c

decompression.o: decompression.c
	gcc -c decompression.c

ezip.o: ezip.c
	gcc -c ezip.c

remove_o: 
	rm ezip.o compression.o decompression.o

clean: 
	rm ezip
