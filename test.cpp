#include "decode.h"

using namespace std;

fetch_registers regs;
decode_registers alu_regs;
global_registers global_regs;

void setMessage(message* mex, string& source, string& dest, void* magic) {
	strcpy(mex->source,(const char*)source.c_str());
	strcpy(mex->dest,(const char*)dest.c_str());
	mex->magic_struct = magic;
	mex->next = NULL;
}

int main(){
	string s = "Decode";
	Decode* dec = new Decode(s,0);
	message * mex = new message;
	string fetch_s(FETCH);
	string decode_s(DECODE);
	event  ev;
	vector<event*> event_list;
	ev.time = 0;
	string message1 = "DECODE send message to ";

	cout << "Start test" << endl;

	cout << "FETCH send messages to DECODE: " << endl;

	cout << "\t Format 0" << endl;

	cout << "\t \t NOP -> ";

	regs.opcode = 00000001;

	setMessage(mex, fetch_s, decode_s, NULL);

	ev.m = mex;

	event_list = dec->notify(&ev);

	cout << message1 << event_list[0]->m->dest << endl;

	return 0;
}