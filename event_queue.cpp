#include "event_queue.h"
#include <iostream>
using namespace std;


int main() {
	event_queue e;
}

void event_queue::insert(event * p) {
	event* prec = 0;
	event* r = this->queue;
	while (r != 0) {
		if(p->time < r->time) {
			p->time -= r->time;
			prec = r;
			r = r->next;
		}
		else break;
	}
	p->next = r;
	if (prec != 0)
		prec->next = p;
	else
		this->queue = p;
	if (r != 0)
		r->time -= p->time;
}

event* event_queue::notify_all(message* m) {
	m->timestamp = this->timestamp;
	//prototype

	return NULL;
}

void event_queue::tick()
{
	this->timestamp++;
	if (this->queue != 0)
		this->queue->time--;
	while (this->queue != 0 && this->queue->time == 0) {
		event* r = notify_all(this->queue->m);
		while (r!=0) {
			event* p = r->next;
			r->next = 0;
			this->insert(r);
			r = p;
		}
	}
	if(this->timestamp <=100000000)
		tick();
}
