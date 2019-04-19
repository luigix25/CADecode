#include "decode.h"

using namespace std;

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

char * printResult(string from_to, event* ev) {
	string mexDecode = "DECODE send message to ";
	string ret = ev->m->dest;
	cout << from_to << mexDecode << ret << endl;
	delete ev->m;
	delete ev;
	return (char *)ret.c_str();
}

int main(){
	string fetch_s(FETCH);
	string decode_s(DECODE);
	string ALU_s(ALU);
	string mem_s(MEMORY);

	vector<event*> event_list;	
	Decode* dec = new Decode("Decode",0);
	memory_message* mem_mex;
	int error = 0;

	string mexFD = "\t  FETCH send messages to DECODE -> ";
	string mexAD = "\t  ALU send messages to DECODE -> ";
	string mexMD = "\t  MEMORY send messages to DECODE -> ";
	string error_msgs = "\t\t\t[ERR] wrong destination";

	int nameJump_length = 18;
	string nameJump[nameJump_length] = {	
									"JE","JNE","JA","JAE","JB","JBE","JG","JGE","JL",
									"JLE","JZ","JNZ","JC","JNC","JO","JNO","JS","JNS"
									};
	int nameF2_length = 4;
	string nameF2[nameF2_length] = {"INC", "DEC", "NEG", "NOT"};

	int nameF3F4_length = 15;
	string nameF3F4[nameF3F4_length] = {
					"ADD","SUB","CMP","MUL","IMUL","DIV","IDIV",
 					"AND","OR","SHL","SAL","SHR","SAR","LOAD","STORE"
 					};

	cout << "Legenda: INPUT -> OUTPUT" <<endl; 
	cout << "***** START *****" << endl;

	//F0
	cout << "Format 0" << endl; 
	cout << "\tNOP" << endl;
	regs.opcode = 0x01;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	if(strcmp(printResult(mexFD, event_list[0]), (char *)fetch_s.c_str())){
		error++;
		cout << error_msgs << endl; 
	}

	//F1
	cout << "Format 1" << endl;
	for(int i = 0; i<nameJump_length; i++){
		cout << "\t" << nameJump[i] << endl;
		regs.opcode = 0x20 + i;
		event_list = getEventList(dec,fetch_s, decode_s, NULL);
		if(strcmp(printResult(mexFD, event_list[0]), (char *)fetch_s.c_str())){
			error++;
			cout << error_msgs << endl; 
		}
	}

	//F2
	cout << "Format 2" << endl;
	for(int i = 0; i<nameJump_length; i++){
		cout << "\t" << nameJump[i] << endl;
		regs.opcode = 0x40 + i;
		event_list = getEventList(dec,fetch_s, decode_s, NULL);
		if(strcmp(printResult(mexFD, event_list[0]), (char *)fetch_s.c_str())){
			error++;
			cout << error_msgs << endl; 
		}
	}

	for(int i = 0; i<nameF2_length; i++){
		cout << "\t" << nameF2[i] << endl;
		regs.opcode = 0x52 + i;		
		// Send frome FETCH TO DECODE
		event_list = getEventList(dec,fetch_s, decode_s, NULL);
		if(strcmp(printResult(mexFD, event_list[0]), (char *)ALU_s.c_str())){
			error++;
			cout << error_msgs << endl; 
		}

		// DECODE must send message to ALU, then ALU send message to DECODE
		event_list = getEventList(dec,ALU_s, decode_s, NULL);
		if(strcmp(printResult(mexAD, event_list[0]), (char *)fetch_s.c_str())){
			error++;
			cout << error_msgs << endl; 
		}
	}

	//F3
	cout << "Format 3" << endl;

	cout << "\t" << "MOV" << endl;
	regs.opcode = 0x60;		
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	if(strcmp(printResult(mexFD, event_list[0]), (char *)fetch_s.c_str())){
		error++;
		cout << error_msgs << endl; 
	}

	for(int i = 0; i<nameF3F4_length; i++){
		cout << "\t" << nameF3F4[i] << endl;
		regs.opcode = 0x61 + i;		
		// Send frome FETCH TO DECODE
		event_list = getEventList(dec,fetch_s, decode_s, NULL);

		if(i<13){
			if(strcmp(printResult(mexFD, event_list[0]), (char *)ALU_s.c_str())){
				error++;
				cout << error_msgs << endl; 
			}
			// DECODE must send message to ALU, then ALU send message to DECODE
			event_list = getEventList(dec,ALU_s, decode_s, NULL);
			if(strcmp(printResult(mexAD, event_list[0]), (char *)fetch_s.c_str())){
				error++;
				cout << error_msgs << endl; 
			}
		} else{
			if(strcmp(printResult(mexFD, event_list[0]), (char *)mem_s.c_str())){
				error++;
				cout << error_msgs << endl; 
			}
			// MEM to DECODE
			mem_mex = (memory_message*) event_list[0]->m->magic_struct;
			event_list = getEventList(dec,mem_s, decode_s, (void*)mem_mex);
			if(strcmp(printResult(mexMD, event_list[0]), (char *)fetch_s.c_str())){
				error++;
				cout << error_msgs << endl; 
			}
		}
	}

	//F4
	cout << "Format 4" << endl;

	cout << "\t" << "MOV" << endl;
	regs.opcode = 0x80;		
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	if(strcmp(printResult(mexFD, event_list[0]), (char *)fetch_s.c_str())){
		error++;
		cout << error_msgs << endl; 
	}


	for(int i = 0; i<nameF3F4_length; i++){
		cout << "\t" << nameF3F4[i] << endl;
		regs.opcode = 0x81 + i;		
		// Send frome FETCH TO DECODE
		event_list = getEventList(dec,fetch_s, decode_s, NULL);

		if(i<13){
			if(strcmp(printResult(mexFD, event_list[0]), (char *)ALU_s.c_str())){
				error++;
				cout << error_msgs << endl; 
			}
			// DECODE must send message to ALU, then ALU send message to DECODE
			event_list = getEventList(dec,ALU_s, decode_s, NULL);
			if(strcmp(printResult(mexAD, event_list[0]), (char *)fetch_s.c_str())){
				error++;
				cout << error_msgs << endl; 
			}
		} else{	
			if(strcmp(printResult(mexFD, event_list[0]), (char *)mem_s.c_str())){
				error++;
				cout << error_msgs << endl; 
			}		
			// MEM to DECODE
			mem_mex = (memory_message*) event_list[0]->m->magic_struct;
			event_list = getEventList(dec,mem_s, decode_s, (void*)mem_mex);
			if(strcmp(printResult(mexMD, event_list[0]), (char *)fetch_s.c_str())){
				error++;
				cout << error_msgs << endl; 
			}
		}
	}

	cout << "\t" << "XCHG" << endl;
	regs.opcode = 0x90;		
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	if(strcmp(printResult(mexFD, event_list[0]), (char *)fetch_s.c_str())){
		error++;
		cout << error_msgs << endl; 
	}

	cout << "***** END *****" << endl;
	cout << "Errors: " << error << endl;
	delete dec;
	return 0;
}

