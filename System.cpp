#include <algorithm>
#include <cstring>

#include "System.h"

using namespace std;

//Bind the module to the system, so onNotify() can be called
void System::addModule(module* m) {
	this->modules.push_back(m);
	sort(modules.begin(), modules.end(), [](module* left, module* right){return left->getPriority() > right->getPriority();});
}

void System::insert(event* p) {
	if(p->time >= this->currentTime){
		this->queue.push(p);
	}
}	
	
vector<event*> System::notify_all(event* e) {
	vector<event*> tempEvents;
	vector<event*> newEvents;
	
	//Notify all the modules while the message is valid (A message can become invalid if it was intercepted and digested. E.g.: A message from CPU to MEMORY intercepted by the CACHE)
	for(int i=0; i<modules.size() && e->m != NULL && e->m->valid; i++){
		tempEvents = modules[i]->notify(e);
		while(tempEvents.size() > 0){
			newEvents.push_back(tempEvents.back());//back() because std::vector does not support front pop
			tempEvents.pop_back();
		}
	}
	
	return newEvents;
}

//Launches the simulation
void System::run() {
	vector<event*> tempEvents;
	for(int i=0; i<modules.size(); i++){
		tempEvents = modules[i]->initialize();
		while(tempEvents.size() > 0){
			insert(tempEvents.back());//back() because std::vector does not support front pop
			tempEvents.pop_back();
		}
	}
	
	while(!this->queue.empty()){
		tick();
	}
	//If there are no more events, the program terminates
}

void System::tick() {
	this->currentTime = this->queue.top()->time;
	//Notify all the modules about the next event and remove it from the queue
	vector<event*> result = notify_all(this->queue.top());
	//Deallocate the message and the event
	delete this->queue.top()->m;
	delete this->queue.top();
	this->queue.pop();
	
	//Schedule all the events generated in this tick
	while (result.size() > 0) {
		this->insert(result.back());//back() because std::vector does not support front pop
		result.pop_back();
	}
}
