// vim: set ts=4 nu ai:
#ifndef CACHE_H
#define CACHE_H

#include <portability.h>
#include <dram.h>
#define SETS 256
#define SETBITS 8
#define TAGBITS 22
#define WAYS 1
#define LATENCY 30

struct cache_index{
	CORE_UINT(TAGBITS) tag;
	CORE_UINT(SETBITS) set;
	CORE_UINT(1) dirtybit;
};

class Cache{

	private:
		cache_index index[SETS];
		CORE_UINT(8) cache[SETS][4];
		Dram* dram_location;

	public:
		//Instantiate cache with pointer to DRAM object
		Cache(Dram* dram);

		void store(CORE_UINT(32) address, CORE_INT(32) value, CORE_UINT(2) op);
			
		CORE_INT(32) load(CORE_UINT(32) address, CORE_UINT(2) op, CORE_UINT(1) sign);

		CORE_UINT(TAGBITS) getTag(CORE_UINT(32) address);
		
		CORE_UINT(SETBITS) getSet(CORE_UINT(32) address);

		CORE_UINT(2) getOffset(CORE_UINT(32) address);

};


#endif /* CACHE_H */
