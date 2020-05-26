CC = gcc
CFLAGS = -Wall -g
DEPS = bitstream.h
STRDUMP_OBJ = str_dump.o bitstream.o
IMGDUMP_OBJ = imagedump.o bitstream.o
STREAM_TEST_OBJ = bitstream_test.o

# make .o files from the .c files that have the same basename
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: str_dump bitstream_test imagedump

str_dump: $(STRDUMP_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

bitstream_test: $(STREAM_TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

imagedump: $(IMGDUMP_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
