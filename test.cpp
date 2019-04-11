#include "decode.h"

using namespace std;

int main(){
	string s = "Decode";
	Decode* dec = new Decode(s,0);
	message * mex = new message;
	event  ev;
	vector<event*> event_list;
	ev.time = 0;

	cout << "Start test" << endl;

	cout << "Format 0" << endl;

	cout << "\t NOP ... ";

	regs.opcode = 00000001;

	strcpy(mex->source,FETCH);
	strcpy(mex->dest,DECODE);
	mex->magic_struct = NULL;
	mex->next = NULL;

	ev.m = mex;

	event_list = dec->notify(&ev);

	cout << event_list[0]->m->source << endl;



	return 0;
}