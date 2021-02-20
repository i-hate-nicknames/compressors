CC = gcc
CFLAGS = -Wall -g

SRC_DIR = src

BITSTREAM_DEP = bitstream/bitstream.h bitstream/bitstream.o
CMDUTIL_DEP = cmdutil/cmdutil.o cmdutil/cmdutil.h

RL_DEPS_RAW = runlength/run_length.o $(BITSTREAM_DEP) $(CMDUTIL_DEP)
RL_DEPS = $(addprefix $(SRC_DIR)/, $(RL_DEPS_RAW))

run_length: $(RL_DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

IMGDUMP_DEPS_RAW = $(BITSTREAM_DEP) imagedump/imagedump.o
IMGDUMP_DEPS = $(addprefix $(SRC_DIR)/, $(IMGDUMP_DEPS_RAW))

imagedump: $(IMGDUMP_DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

strdump: src/strdump/str_dump.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
