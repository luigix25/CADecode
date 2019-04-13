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

int main(){
	string fetch_s(FETCH);
	string decode_s(DECODE);
	vector<event*> event_list;	
	Decode* dec = new Decode("Decode",0);

	string message1 = "DECODE send message to ";
	cout << "Start test" << endl;
	cout << "FETCH send messages to DECODE: " << endl;
	cout << "\t Format 0" << endl;

	//F0: NOP
	cout << "\t \t NOP -> ";
	regs.opcode = 00000001;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	cout << message1 << event_list[0]->m->dest << endl;



	return 0;
}