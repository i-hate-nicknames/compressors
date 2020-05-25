CC = gcc
CFLAGS = -Wall -g
DEPS = # .h files will go here
C_OBJ = compressor.o
IMGDUMP_OBJ = imagedump.o bitstream.o
STREAM_TEST_OBJ = bitstream_test.o

# make .o files from the .c files that have the same basename
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: compressor bitstream_test imagedump clean

# make compressor binary
compressor: $(C_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

bitstream_test: $(STREAM_TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

imagedump: $(IMGDUMP_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
