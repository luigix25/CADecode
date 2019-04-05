//decode.h

#define CF 0
#define ZF 6
#define SF 7
#define OF 11

#define EVENT_TIME 30	//in cosa si misurano?? 

#include "module.h"
#include <iostream>
using namespace std;

struct decode_to_mem{
	bool type; // 0 = lettura, 1 = scrittura
	uint16_t address;
	uint16_t data; //questo campo ha significato solo in caso di scrittura
};

decode_to_mem shared_dec_mem;		//nome temporaneo
