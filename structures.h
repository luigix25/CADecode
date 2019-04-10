#pragma once
#include <string>
#include <vector>

using namespace std;

struct message {
	int id;
	bool valid = 0;
	int timestamp = 0; //When this message was created
	char source[10];
	char dest[10];
	void* magic_struct = 0;
	message* next = 0; //this might be NULL
};

struct event{
	int time; //When this event must be dequeued
	message* m;
};