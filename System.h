#pragma once
#include <queue>
#include <vector>

#include "module.h"
#include "structures.h"

using namespace std;

//Custom comparator used by priority_queue
class comparator{
	public:
		int operator()(event* left, event* right) {
			return left->time > right->time;
		}
};

class System
{
	vector<module*> modules;
	priority_queue <event*, std::vector<event*>, comparator > queue;
	
	void insert(event* e);
	vector<event*> notify_all(event* e);
	void tick();
public:
	void addModule(module* m);
	void run();
	int currentTime = 0;
};
