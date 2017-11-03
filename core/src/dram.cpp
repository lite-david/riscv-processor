// vim: set ts=4 nu ai:
#include <dram.h>
#include <stdlib.h>
#include <iostream>

void Dram::setMemory(CORE_UINT(32) address, CORE_UINT(8) value){
	CORE_UINT(30) location = address >> 2;
	CORE_UINT(2) byte = address.SLC(2,0);
	
	if(memory[location].size() == 0){
		memory[location] = std::vector<CORE_UINT(8)>(4,0);
	}
	memory[location][byte] = value;		
}

void Dram::setMemory(CORE_UINT(32) address, CORE_UINT(8) value, CORE_UINT(2) byte){
	if(memory[address].size() == 0){
		memory[address] = std::vector<CORE_UINT(8)>(4,0);
	}
	memory[address][byte] = value;
}

CORE_UINT(8) Dram::getMemory(CORE_UINT(32) address, CORE_UINT(2) byte){
	if(memory[address].size() == 0){
		//std::cout << "Error! Invalid address access in Dram. Terminating" 
		//<< std::endl;
		//exit(-1);
		return 0;
	}
	return memory[address][byte];
}

