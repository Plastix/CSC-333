CC=mpicc
CFLAGS=-I.
DEPS = 
OBJ = mpicell.o CellAut.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mpicell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


clean:
	rm -f *.o *~ 
