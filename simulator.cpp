#include "ExampleModule.h"
#include "System.h"

using namespace std;

int main() {
	System system;
	
	//Initialize the system here by adding modules to the system object
	system.addModule(new ExampleModule("Alice"));
	system.addModule(new ExampleModule("Bob", 10));//The second parameter is the priority. It's useful only when you need to intercept someone's else message
	
	//Call run() to start the simulation
	system.run();
}
