CC = gcc
CFLAGS = -Wall -g
DEPS = # .h files will go here
OBJ = compressor.o

# make .o files from the .c files that have the same basename
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# make compressor binary
compressor: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
