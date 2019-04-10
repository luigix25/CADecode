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
#include "fetch_registers.h"
#include "decode_register.h"
#include <iostream>
#include <cstring>

using namespace std;


uint16_t flag;

/*uint32_t ip;
uint16_t flag;
uint16_t source_reg;			//controllare dimensioni
uint8_t dest_reg;
uint8_t opcode;
*/

uint16_t REGS[6];


struct decode_to_mem{
	bool type; // 0 = lettura, 1 = scrittura
	uint16_t address;
	uint16_t data; //questo campo ha significato solo in caso di scrittura
};

class Decode : public module{
	private:
		bool extract_flag(uint8_t);
		message* format_0();
		message* format_1();
		message* format_2();
		message* format_3();
		message* format_4();

		uint8_t getFormat();
		uint8_t getID();

		message* create_message_for_fetch();
		message* create_message_for_ALU();
		message* create_message_for_memory(bool,uint16_t,uint16_t);

		message* handle_fetch(message *);
		message* handle_alu(message *);
		message* handle_memory(message *);
		message* handle_load(uint16_t);

		decode_to_mem shared_dec_mem;		//nome temporaneo
	public:
		void onNotify(message*);


};
