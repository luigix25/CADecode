/*

Memory cell's length is 32bit
Most significant			Least Significant

 |           |      'source'         |   'dest'  |
 |--OPCODE---|--------1st operand----|---2nd op--|
 |___byte1___|__byte2____|___byte3___|__byte4____|
0           7 8        15 16       23 24       31


Fetch module interface registers for decode_execute module */
#pragma once
struct fetch_registers{
	uint16_t ip; //instruction pointer
	uint32_t mdr; //The 4 bytes retrieved from memory during fetch phase will be stored here
	uint16_t mar; //This 'register' will be used during a communication with memory
				//to indicate the memory address of the instruction that we want to fetch
	uint8_t opcode; //Opcode (byte1) after fetch phase will be stored here
	uint16_t source; //1st operand will be stored here
	uint8_t dest; //2nd operand will be stored here
} regs;

