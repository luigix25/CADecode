CFLAGS= -Wall -std=c++14 -g

all: test

test: decode.o decode.h test.o module.o
	g++ $(CFLAGS) module.o decode.o test.o -o test

test.o: test.cpp decode.h 
	g++ $(CFLAGS) -c test.cpp -o test.o

decode.o: decode.cpp decode.h structures.h fetch_registers.h decode_registers.h memory_message.h
	g++ $(CFLAGS) -c decode.cpp -o decode.o

module.o: module.cpp module.h structures.h
	g++ $(CFLAGS) -c module.cpp -o module.o
clean:
	rm -f *.o test
