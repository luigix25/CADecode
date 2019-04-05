//decode.h
#define CF 0
#define ZF 6
#define SF 7
#define OF 11

#define EVENT_TIME 30	//in cosa si misurano?? 
#define MEMORY "MEM_SUB"
#define ALU "ALU"
#define FETCH "FETCH"
#define DECODE "DECODE"


#include "module.h"
#include <iostream>
#include <cstring>

using namespace std;

uint32_t ip;
uint16_t flag;
uint16_t source_reg;			//controllare dimensioni
uint8_t dest_reg;
uint8_t opcode;

uint16_t REGS[6];


uint16_t op1_ALU;
uint16_t op2_ALU;
uint8_t dest_ALU;

struct decode_to_mem{
	bool type; // 0 = lettura, 1 = scrittura
	uint16_t address;
	uint16_t data; //questo campo ha significato solo in caso di scrittura
};

class Decode : public module{
	private:
		bool extract_flag(uint8_t);
		event* format_0();
		event* format_1();
		event* format_2();
		event* format_3();
		event* format_4();

		uint8_t getFormat();
		uint8_t getID();

		event* create_event_for_fetch();
		event* create_event_for_ALU();
		event* create_event_for_memory(bool,uint16_t,uint16_t);

		event* handle_fetch(message *);
		event* handle_alu(message *);
		event* handle_memory(message *);
		event* handle_load(uint16_t);

		decode_to_mem shared_dec_mem;		//nome temporaneo


	public:
		event* notify(message*);


};
