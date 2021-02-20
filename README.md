# Compressors
A playground for different compression techniques. The design is heavily inspired by chapter on
compression in Algorithms by Robert Sedgewick et al.

# Tools
Different programs and library code that is used for implementing the algorithms themselves, or
for testing binary output

### Bit streams
Bit streams library (bitstream.c, bitstream.h) is a wrapper around FILE that allows reading and writing
bits. Compression algorithms make use of it because they usually want writing to be on a finer level
than bytes.

### Image dump
Image dump is a program that reads input and produces a black and white image grid. Black squares
represent 1s and white squares 0s. Useful to debug and get intuition about the output of an algorithm.

### String dump
String dump prints given input as a string of 0s and 1s, least significant bit of each byte comes first.

# Compression algorithms

### Run-length encoding
Run-length is an easy algorithm that works well when there are long sequences of repeating bits.
The idea is to put a number of each value repeated instead of repeating the value, so input
aaabbcccccc can be translated as 3a2b6c.
In this implementation we are encoding bit values of 0 and 1. It would be cumbersome to encode
the values themselves into the output, so instead we just put number of repeats as integers.
We assume we start with 1, and the next number is the number of 0s, then 1s and so on. So, for
integer input 5,3,4 we can conclude that the original data is 111110001111.
Sequences longer than 255 cannot be encoded, so a sequence of length n > 255 are encoded like
this: 255,0,encode(n-255). Basically we output 0 as a zero-length sequence of the other
value and then continue encoding whatever remains.
