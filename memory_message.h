#pragma once
struct memory_message{
	bool 		type;						//0 for read 1 for write
	uint16_t 	address;
	uint16_t 	data;						
};


/*
	All the operations need an ACK.

	In the case of a read operation, the data field is empty, it will contain the actual value read from the memory in the response from the cache(?) module.
	
	In the case of a write operation, all the fields are used. In the ACK message the magic_struct ( of the event structure ) is NULL.


*/