// vim: set ts=4 nu ai:
#ifndef DRAM_H
#define DRAM_H

#include <portability.h>
#include <map>
#include <vector>

class Dram{
	
	private:
		std::map<CORE_UINT(32), std::vector<CORE_UINT(8)> > memory;

	public:
		
		void setMemory(CORE_UINT(32) address, CORE_UINT(8) value, CORE_UINT(2) byte);
		void setMemory(CORE_UINT(32) address, CORE_UINT(8) value);
		CORE_UINT(8) getMemory(CORE_UINT(32) address, CORE_UINT(2) byte);

};

#endif /* DRAM_H */
