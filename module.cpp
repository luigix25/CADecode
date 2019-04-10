#include <string>
#include <vector>

#include "module.h"

int module::_messageId = 0;

module::module(string name, int priority){//Default priority is 0. Higher the value, higher the priority. Negative values are allowed. Modules with higher priority are notified first.
	_name = name;
	_priority = priority;
	_currentTime = 0;
}

string module::getName(){return _name;};

int module::getPriority(){return _priority;}

//This method is used by the system, should not be called directly
vector<event*> module::initialize(){
	vector<event*> result = events;
	events.clear();
	return result;
};

//This method is used by the system, should not be called directly
vector<event*> module::notify(event* e){
	_currentTime = e->time;
	onNotify(e->m);
	vector<event*> result = events;
	events.clear();
	return result;
};

//Returns the current simulated system time
int module::getTime(){
	return _currentTime;
}

//Custom modules must push their messages using this function (delay is optional)
void module::sendWithDelay(message* m, int delay){
	if(delay < 0)
		return;
	
	m->id = ++_messageId;
	event* e = new event();
	e->time = getTime() + delay;
	e->m = m;
	events.push_back(e);
}
