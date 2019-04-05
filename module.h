#pragma once
#include "event_queue.h"
class module
{
public:
	virtual event* notify(message* m);
};

