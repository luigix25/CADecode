#include "decode.h"
#include "System.h"

using namespace std;

int main() {
	System system;
	
	//Initialize the system here by adding modules to the system object
	system.addModule(new Decode("Decode"));
	//Call run() to start the simulation
	system.run();
}
