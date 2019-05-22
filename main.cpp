#include "decode.h"
#include "main.h"

using namespace std;

fetch_registers regs;
decode_registers alu_regs;
global_registers global_regs;

int main() {
	System system;
	
	int priority = 0;
	system.addModule(new Decode("Decode", priority));
	system.run();
}