CC=gcc
CFLAGS=-I.
DEPS =
OBJ = CellAut.o 

%.o: %.c $(DEPS)
	$(CC) -fopenmp -c -o $@ $< $(CFLAGS)

runcellmp: $(OBJ) runcellmp.o
	$(CC) -fopenmp -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o *~
