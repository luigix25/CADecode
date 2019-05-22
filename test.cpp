#include "decode.h"
#include "main.h"

using namespace std;
#define nameF3F4_length 15
#define nameF2_length 	4
#define nameJump_length 18


fetch_registers regs;
decode_registers alu_regs;
global_registers global_regs;

vector<event*> getEventList(Decode* dec, string& source, string& dest, void* magic) {
	message * mex = new message;
	strcpy(mex->source,(const char*)source.c_str());
	strcpy(mex->dest,(const char*)dest.c_str());
	mex->magic_struct = magic;
	mex->next = NULL;
	event  ev;
	ev.time = 0;
	ev.m = mex;
	return dec->notify(&ev);
}

int printResult(string from_to, event* ev, string expected) {
	string mexDecode = "DECODE send message to ";
	cout << from_to << mexDecode << ev->m->dest << endl;
	if(strcmp((char *) expected.c_str(), ev->m->dest)){
			cout << "\t [ERR] Destination is " << ev->m->dest << " instead of " << expected << endl; 
			delete ev->m;
			delete ev;
			return 1;
	}
	delete ev->m;
	delete ev;
	return 0;
}

int main(){
	string fetch_s(FETCH);
	string decode_s(DECODE);
	string ALU_s(ALU);
	string mem_s(MEMORY);

	vector<event*> event_list;	
	Decode* dec = new Decode("Decode",0);
	memory_message* mem_mex;
	int error_count_mex = 0;
	int error_count_reg = 0;

	bool error;

	memory_message * mm;

	string mexFD = "\t  FETCH send messages to DECODE -> ";
	string mexAD = "\t  ALU send messages to DECODE -> ";
	string mexMD = "\t  MEMORY send messages to DECODE -> ";

	//int nameJump_length = 18;
	string nameJump[nameJump_length] = {	
									"JE","JNE","JA","JAE","JB","JBE","JG","JGE","JL",
									"JLE","JZ","JNZ","JC","JNC","JO","JNO","JS","JNS"
									};
	//int nameF2_length = 4;
	string nameF2[nameF2_length] = {"INC", "DEC", "NEG", "NOT"};

	//int nameF3F4_length = 15;
	string nameF3F4[nameF3F4_length] = {
					"ADD","SUB","CMP","MUL","IMUL","DIV","IDIV",
 					"AND","OR","SHL","SAL","SHR","SAR","LOAD","STORE"
 					};

	cout << "Legenda: INPUT -> OUTPUT" <<endl; 
	cout << "***** START MESSAGES TEST*****" << endl;

	//F0
	cout << "Format 0" << endl; 
	cout << "\tNOP" << endl;
	regs.opcode = 0x01;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	error_count_mex += printResult(mexFD, event_list[0], fetch_s);

	//F1
	cout << "Format 1" << endl;
	for(int i = 0; i<nameJump_length; i++){
		cout << "\t" << nameJump[i] << endl;
		regs.opcode = 0x20 + i;
		event_list = getEventList(dec,fetch_s, decode_s, NULL);
		error_count_mex += printResult(mexFD, event_list[0], fetch_s);
	}

	cout << "\t" << "JMP" << endl;
	regs.opcode = 0x32;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	error_count_mex += printResult(mexFD, event_list[0], fetch_s);

	//F2
	cout << "Format 2" << endl;
	for(int i = 0; i<nameJump_length; i++){
		cout << "\t" << nameJump[i] << endl;
		regs.opcode = 0x40 + i;
		event_list = getEventList(dec,fetch_s, decode_s, NULL);
		error_count_mex += printResult(mexFD, event_list[0], fetch_s);
	}

	for(int i = 0; i<nameF2_length; i++){
		cout << "\t" << nameF2[i] << endl;
		regs.opcode = 0x52 + i;		
		// Send frome FETCH TO DECODE
		event_list = getEventList(dec,fetch_s, decode_s, NULL);
		error_count_mex += printResult(mexFD, event_list[0], ALU_s);

		// DECODE must send message to ALU, then ALU send message to DECODE
		event_list = getEventList(dec,ALU_s, decode_s, NULL);
		error_count_mex += printResult(mexAD, event_list[0], fetch_s);
	}

	cout << "\t" << "JMP" << endl;
	regs.opcode = 0x56;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	error_count_mex += printResult(mexFD, event_list[0], fetch_s);

	//F3
	cout << "Format 3" << endl;

	cout << "\t" << "MOV" << endl;
	regs.opcode = 0x60;		
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	error_count_mex += printResult(mexFD, event_list[0], fetch_s);

	for(int i = 0; i<nameF3F4_length; i++){
		cout << "\t" << nameF3F4[i] << endl;
		regs.opcode = 0x61 + i;		
		// Send frome FETCH TO DECODE
		event_list = getEventList(dec,fetch_s, decode_s, NULL);

		if(i<13){
			error_count_mex += printResult(mexFD, event_list[0], ALU_s);
			// DECODE must send message to ALU, then ALU send message to DECODE
			event_list = getEventList(dec,ALU_s, decode_s, NULL);
			error_count_mex += printResult(mexAD, event_list[0], fetch_s);
		} else{
			mem_mex = (memory_message*) event_list[0]->m->magic_struct;
			error_count_mex += printResult(mexFD, event_list[0], mem_s);
			// MEM to DECODE

			event_list = getEventList(dec,mem_s, decode_s, (void*)mem_mex);
			error_count_mex += printResult(mexMD, event_list[0], fetch_s);
		}
	}

	//F4
	cout << "Format 4" << endl;

	cout << "\t" << "MOV" << endl;
	regs.opcode = 0x80;		
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	error_count_mex += printResult(mexFD, event_list[0], fetch_s);


	for(int i = 0; i<nameF3F4_length; i++){
		cout << "\t" << nameF3F4[i] << endl;
		regs.opcode = 0x81 + i;		
		// Send frome FETCH TO DECODE
		event_list = getEventList(dec,fetch_s, decode_s, NULL);

		if(i<13){
			error_count_mex += printResult(mexFD, event_list[0], ALU_s);
			// DECODE must send message to ALU, then ALU send message to DECODE
			event_list = getEventList(dec,ALU_s, decode_s, NULL);
			error_count_mex += printResult(mexAD, event_list[0], fetch_s);
		} else{	
			mem_mex = (memory_message*) event_list[0]->m->magic_struct;
			error_count_mex += printResult(mexFD, event_list[0], mem_s);		
			// MEM to DECODE
		
			event_list = getEventList(dec,mem_s, decode_s, (void*)mem_mex);
			error_count_mex += printResult(mexMD, event_list[0], fetch_s);
		}
	}

	cout << "\t" << "XCHG" << endl;
	regs.opcode = 0x90;		
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	error_count_mex += printResult(mexFD, event_list[0], fetch_s);

	cout << "***** END MESSAGES TEST *****" << endl;

	cout << "***** START REGISTERS TEST *****" << endl;

	// Test conditional JUMP: JBE F1
	cout << "\t F1 " << "JBE registers test: ";
	regs.opcode = 0x25;
	global_regs.flag = 0x0021;
	regs.ip = 0x0000;
	regs.source = 0x0010;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	if(regs.ip != regs.source){
		error_count_reg++;
		cout << "\t [ERR] Wrong ip after message" << endl;
	} else{
		cout << "\t OK" << endl;
	}
	delete event_list[0]->m;
	delete event_list[0];
	// Test conditional JUMP: JBE F2
	cout << "\t F2 " << "JBE registers test: ";
	regs.opcode = 0x45;
	global_regs.flag = 0x0021;
	regs.ip = 0x0000;
	regs.source = 0x0000; //AX
	global_regs.general_regs[regs.source] = 0x0010;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	if(regs.ip != global_regs.general_regs[regs.source]){
		error_count_reg++;
		cout << "\t [ERR] Wrong ip after message" << endl;
	} else{
		cout << "\t OK" << endl;
	}
	delete event_list[0]->m;
	delete event_list[0];
	// Test INC F2 
	cout << "\t F2 " << "INC registers test: ";
	regs.opcode = 0x52;
	regs.source = 0x0000; //AX
	global_regs.general_regs[regs.source] = 0xfffe;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	error = false;	
	if(alu_regs.operand1 != global_regs.general_regs[regs.source]){
		error = true;
		cout<< endl << "\t [ERR] Wrong operand1 in ALU reg";
	}

	if(alu_regs.destination_reg != regs.source){
		error = true;
		cout << endl  << "\t [ERR] Wrong destination_reg in ALU reg";
	}

	if(alu_regs.opcode !=  regs.opcode){
		error = true;
		cout << endl  << "\t [ERR] Wrong opcode in ALU reg";
	}

	if(error){
		error_count_reg++;
		cout << endl;
	} else{
		cout << "\t OK" << endl;
	}
	delete event_list[0]->m;
	delete event_list[0];
	// Test MOV F3
	cout << "\t F3 " << "MOV registers test: ";
	regs.opcode = 0x60;
	regs.source = 0x0011; // immediato
	regs.dest = 0x0001; // BX
	global_regs.general_regs[regs.dest] = 0x0000;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	if(global_regs.general_regs[regs.dest] != regs.source){
		error_count_reg++;
		cout << "\t [ERR] Wrong reg value after message" << endl;
	} else{
		cout << "\t OK" << endl;
	}
	delete event_list[0]->m;
	delete event_list[0];
	// Test ADD F3
	cout << "\t F3 " << "ADD registers test: ";
	regs.opcode = 0x61;
	regs.source = 0xfffe; 
	regs.dest = 0x01; //BX 
	global_regs.general_regs[regs.dest] = 0x0001;

	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	error = false;	
	if(alu_regs.operand1 != regs.source){
		error = true;
		cout<< endl << "\t [ERR] Wrong operand1 in ALU reg";
	}

	if(alu_regs.operand2 != global_regs.general_regs[regs.dest]){
		error = true;
		cout<< endl << "\t [ERR] Wrong operand2 in ALU reg";
	}

	if(alu_regs.destination_reg != regs.dest){
		error = true;
		cout << endl  << "\t [ERR] Wrong destination_reg in ALU reg";
	}

	if(alu_regs.opcode !=  regs.opcode){
		error = true;
		cout << endl  << "\t [ERR] Wrong opcode in ALU reg";
	}

	if(error){
		error_count_reg++;
		cout << endl;
	} else{
		cout << "\t OK" << endl;
	}
	delete event_list[0]->m;
	delete event_list[0];
	// Test STORE F3
	cout << "\t F3 " << "STORE message content test: ";
	regs.opcode = 0x6F;
	regs.source = 0xfefe; 
	regs.dest = 0x02; //CX 
	global_regs.general_regs[regs.dest] = 0x0001;

	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	mm = (memory_message *) event_list[0]->m->magic_struct;

	if(!mm->type){
		error = true;
		cout << endl  << "\t [ERR] Wrong type in memory_message";
	}

	if(mm->address != regs.source){
		error = true;
		cout << endl  << "\t [ERR] Wrong address in memory_message";
	}

	if(mm->data != global_regs.general_regs[regs.dest]){
		error = true;
		cout << endl  << "\t [ERR] Wrong data in memory_message";
	}

	if(error){
		error_count_reg++;
		cout << endl;
	} else{
		cout << " OK" << endl;
	}
	delete event_list[0]->m;
	delete event_list[0];
	// Test MOV F4
	cout << "\t F4 " << "MOV registers test: ";
	regs.opcode = 0x80;
	regs.source = 0x0002; // CX
	global_regs.general_regs[regs.source] = 0x0011;
	regs.dest = 0x0003; // DX
	global_regs.general_regs[regs.dest] = 0x0000;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	if(global_regs.general_regs[regs.dest] != global_regs.general_regs[regs.source]){
		error_count_reg++;
		cout << "\t [ERR] Wrong reg value after message" << endl;
	} else{
		cout << "\t OK" << endl;
	}
	delete event_list[0]->m;
	delete event_list[0];
	// Test ADD F4
	cout << "\t F4 " << "ADD registers test: ";
	regs.opcode = 0x81;
	regs.source = 0x0000; //AX 
	regs.dest = 0x01; //BX 
	global_regs.general_regs[regs.source] = 0xfffe;
	global_regs.general_regs[regs.dest] = 0x0001;

	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	error = false;	
	if(alu_regs.operand1 != global_regs.general_regs[regs.source]){
		error = true;
		cout<< endl << "\t [ERR] Wrong operand1 in ALU reg";
	}

	if(alu_regs.operand2 != global_regs.general_regs[regs.dest] ){
		error = true;
		cout<< endl << "\t [ERR] Wrong operand2 in ALU reg";
	}

	if(alu_regs.destination_reg != regs.dest){
		error = true;
		cout << endl  << "\t [ERR] Wrong destination_reg in ALU reg";
	}

	if(alu_regs.opcode !=  regs.opcode){
		error = true;
		cout << endl  << "\t [ERR] Wrong opcode in ALU reg";
	}

	if(error){
		error_count_reg++;
		cout << endl;
	} else{
		cout << "\t OK" << endl;
	}
	delete event_list[0]->m;
	delete event_list[0];
	// Test STORE F4
	cout << "\t F4 " << "STORE message content test: ";
	regs.opcode = 0x8F;
	regs.source = 0x0001; //BX 
	regs.dest = 0x02; //CX 
	global_regs.general_regs[regs.source] = 0xfefe;
	global_regs.general_regs[regs.dest] = 0x0001;

	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	mm = (memory_message *) event_list[0]->m->magic_struct;

	if(!mm->type){
		error = true;
		cout << endl  << "\t [ERR] Wrong type in memory_message";
	}

	if(mm->address != global_regs.general_regs[regs.dest]){
		error = true;
		cout << endl  << "\t [ERR] Wrong address in memory_message";
	}

	if(mm->data != global_regs.general_regs[regs.source]){
		error = true;
		cout << endl  << "\t [ERR] Wrong data in memory_message";
	}

	if(error){
		error_count_reg++;
		cout << endl;
	} else{
		cout << " OK" << endl;
	}
		delete event_list[0]->m;
	delete event_list[0];

	// Test XCHG F4
	cout << "\t F4 " << "XCHG registers test: ";
	regs.opcode = 0x90;
	regs.source = 0x0002; // CX
	uint16_t oldSourceValue = 0x0011;
	uint16_t oldDestValue = 0x0022;
	global_regs.general_regs[regs.source] = oldSourceValue;
	regs.dest = 0x0003; // DX
	global_regs.general_regs[regs.dest] = oldDestValue;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	delete event_list[0]->m;
	delete event_list[0];
	if(global_regs.general_regs[regs.dest] != oldSourceValue || global_regs.general_regs[regs.source] != oldDestValue){
		error_count_reg++;
		cout << " [ERR] Wrong reg value after message" << endl;
	} else{
		cout << " \t OK" << endl;
	}

	cout << "***** END REGISTERS TEST *****" << endl;
	cout << "Messages errors: " << error_count_mex << endl;
	cout << "Registers errors: " << error_count_reg << endl;
	cout << "------------------" << endl;
	cout << "Total errors: " << error_count_mex + error_count_reg << endl;
	delete dec;
	return 0;
}

