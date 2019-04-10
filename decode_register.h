struct decode_registers{
	uint8_t opcode; 						//opcode
	uint16_t operand1;						//first operand
	uint16_t operand2;						//second operand ( can be 0 )
	uint8_t  destination_reg;				//contains the code of the destination register
} alu_regs;