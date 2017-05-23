
#got some help from http://www.puxan.com/web/howto-write-generic-makefiles/
CC=gcc
CFLAGS=-I. -fopenmp
LDFLAGS= -fopenmp 
USEMATH = -lm

#EXEC = hello 
OBJECTS = $(SOURCES:.c=.o)
SOURCES = $(wildcard *.c)
BINS=$(SOURCES:.c=)  #to compile a target, compile its c


# To obtain object files
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@


all: $(BINS)

project4: project4.o PGM.o iroutines.o p4utils.o
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(USEMATH)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXEC) -lm

clean:
	rm -f $(OBJECTS) $(BINS)
