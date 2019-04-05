CFLAGS= -Wall -std=c++14 -g
LIBFLG = 
LIBDIP = library.o library/library.h filemanager.o
LIBFLS = library.o filemanager.o

all: decode

decode: decode.o decode.h
	g++ $(CFLAGS) decode.o -o decode

decode.o: decode.cpp decode.h
	g++ $(CFLAGS) -c decode.cpp -o decode.o

clean:
	rm -f *.o decode
