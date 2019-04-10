CFLAGS= -Wall -std=c++14 -g
LIBFLG = 
LIBDIP = module.o
LIBFLS = library.o filemanager.o

all: decode

decode: decode.o decode.h structures.h
	g++ $(CFLAGS) $(LIBDIP) decode.o -o decode

decode.o: decode.cpp module.o decode.h structures.h
	g++ $(CFLAGS) -c decode.cpp -o decode.o

module.o: module.cpp module.h structures.h
	g++ $(CFLAGS) -c module.cpp -o module.o
clean:
	rm -f *.o decode
