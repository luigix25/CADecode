#pragma once

#include <string>
#include <vector>

#include "structures.h"

using namespace std;

//Basic module object, custom objects must be inherited from it
class module
{
	static int _messageId;
	string _name;
	int _priority;
	int _currentTime;
	vector<event*> events;
	
	virtual void onNotify(message* m){};
	
protected:
	int getTime();
	void sendWithDelay(message* m, int delay = 0);
	
public:
	module(string name, int priority);
	string getName();
	int getPriority();
	vector<event*> initialize();
	vector<event*> notify(event* e);
};
