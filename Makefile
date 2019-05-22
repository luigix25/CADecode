CFLAGS= -Wall -std=c++14 -g

all: test main

main: decode.o decode.h main.o module.o System.o
	g++ $(CFLAGS) module.o decode.o System.o main.o -o main

test: decode.o decode.h test.o module.o
	g++ $(CFLAGS) module.o decode.o test.o -o test

test.o: test.cpp decode.h main.h 
	g++ $(CFLAGS) -c test.cpp -o test.o

main.o: main.cpp decode.h main.h System.h 
	g++ $(CFLAGS) -c main.cpp -o main.o

system.o: System.cpp System.h module.h structures.h
	g++ $(CFLAGS) -c System.cpp -o System.o

decode.o: decode.cpp decode.h main.h
	g++ $(CFLAGS) -c decode.cpp -o decode.o

module.o: module.cpp module.h structures.h
	g++ $(CFLAGS) -c module.cpp -o module.o
clean:
	rm -f *.o test main
