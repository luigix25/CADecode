//decode.h
#pragma once

#define CF 0
#define ZF 6
#define SF 7
#define OF 11

#define EVENT_TIME 30	//in cosa si misurano?? 
#define MEMORY "LowerLvl"
#define ALU "ALU"
#define FETCH "FETCH"
#define DECODE "DECODE"

#include "module.h"
#include "main.h"

#include <iostream>
#include <cstring>

using namespace std;


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

		message* handle_fetch();
		message* handle_alu();
		message* handle_memory(message *);
		message* handle_load(uint16_t);
		
		memory_message shared_dec_mem;
	public:
		Decode(string, int);
		void onNotify(message*);		
		virtual ~Decode() = default; //Va aggiunto altrimenti da un warning quando si dealloca onNotify
};
