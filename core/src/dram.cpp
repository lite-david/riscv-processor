// vim: set ts=4 nu ai:
#include <dram.h>
#include <vector>

void Dram::setMemory(CORE_UINT(32) address, CORE_UINT(8) value){
	CORE_UINT(30) location = address >> 2;
	CORE_UINT(2) byte = address % 4;
	
	if(memory[location] == 0){
		memory[location] = vector<CORE_UINT(8)>(4);
	}
	memory[location][byte] = value;		
}

void Dram::setMemory(CORE_UINT(32) address, CORE_UINT(8) value, CORE_UINT(2) byte){
	memory[address][byte] = value;
}

CORE_UINT(8) Dram::getMemory(CORE_UINT(32) address, CORE_UINT(2) byte){
	return memory[address][byte];
}

