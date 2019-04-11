#include "decode.h"
//#include "structures.h"
#include <iostream>

using namespace std;

int main(){
	Decode* d = new Decode("Decode");
	message * m = new message;
	event * e = new event;	
	e->time = 0;

	cout << "Start test" << endl;

	cout << "Format 0" << endl;

	cout << "\t NOP ... ";

	regs.opcode = 00000001;

	strcpy(m->source,FETCH);
	strcpy(m->dest,DECODE);
	m->magic_struct = NULL;
	m->next = NULL;

	e->message = m;

	e = d->notify(e);

	cout << e->message->source << endl;



	return 0;
}