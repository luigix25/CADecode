#pragma once

struct message {
	bool valid = 0;
	int timestamp = 0;
	char source[10];
	char dest[10];
	void* magic_struct = 0;
	message* next = 0; //this might be NULL
};

struct event{
	int time;
	message* m;
	event* next;
};

class event_queue
{
private:
	event* queue;
	int timestamp;
	void insert(event* e);
	event* notify_all(message* m);
public:
	event_queue() { queue = 0; }

	void tick();
};
