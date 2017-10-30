// vim: set ts=4 nu ai:
#include <cache.h>
#include <assert.h>

Cache::Cache(Dram* dram){
	assert(SETBITS + TAGBITS + 2 == 32);
	assert(1 << SETBITS == SETS);
	dram_location = dram;
	int i =0;
	for(i=0;i<SETS;i++){
		cache[i][0] = 0;
		cache[i][1] = 0;
		cache[i][2] = 0;		
		cache[i][3] = 0;
		cache_index[i].tag = 0;
		cache_index[i].set = 0;
		cache_index[i].dirtybit=0;
	}
}

void Cache::set(CORE_UINT(32) address, CORE_INT(32) value, CORE_UINT(2) op){
	// For store byte, op = 0
	// For store half word, op = 1
	// For store word, op = 3
	CORE_INT(TAGBITS) tag = getTag(address);
	CORE_INT(SETBITS) set = getSet(address);
	CORE_INT(2) offset = getOffset(address);
	CORE_UINT(8) byte0 = value.SLC(8,0);
	CORE_UINT(8) byte1 = value.SLC(8,8);
	CORE_UINT(8) byte2 = value.SLC(8,16);
	CORE_UINT(8) byte3 = value.SLC(8,24);

	CORE_UINT(32) dram_address = 0;
	dram_address.SET_SLC(0,set);
	dram_address.SET_SLC(SETBITS,tag);
	CORE_UINT(1) tag_match = (tag == cache_index[set].tag) ? 1 : 0;
	
	if(cache_index[set].dirty_bit == 1 && tag_match == 0){
		dram.setMemory(dram_address,cache[set][0],0);
		dram.setMemory(dram_address,cache[set][1],1);
		dram.setMemory(dram_address,cache[set][2],2);
		dram.setMemory(dram_address,cache[set][3],3);
	}

	cache[set][offset] = byte0;
	if(op & 1){
		cache[set][offset+1] = byte1;
	}
	if(op & 2){
		cache[set][2] = byte2;
		cache[set][3] = byte3;
	}
	cache_index[set].set = set;
	cache_index[set].tag = tag;
	cache_index[set].dirtybit = 1;
}

CORE_INT(32) Cache::get(CORE_UINT(32) address, CORE_UINT(2) op, CORE_UINT(1) sign){
	// For load byte, op = 0
	// For load half word, op = 1
	// For load word, op = 3
	
	CORE_INT(TAGBITS) tag = getTag(address);
	CORE_INT(SETBITS) set = getSet(address);
	CORE_INT(2) offset = getOffset(address);
	CORE_UINT(1) tag_match = (tag == cache_index[set].tag) ? 1 : 0;
	
	CORE_INT(32) result;
	result = sign ? -1 : 0;
	
	CORE_UINT(8) byte0 = memory[location][0];
	CORE_UINT(8) byte1 = memory[location][1];
	CORE_UINT(8) byte2 = memory[location][2];
	CORE_INT(8) byte3 = memory[location][3];
	switch(offset){
		case 0:
			break;
		case 1:
			byte0 = byte1;
			break;
		case 2:
			byte0 = byte2;
			byte1 = byte3;
			break;
		case 3:
			byte0 = byte3;
			break;
		}
	result.SET_SLC(0,byte0);
	if(op & 1){
		result.SET_SLC(8,byte1);		}
	}
	if(op & 2){
		result.SET_SLC(16,byte2);
		result.SET_SLC(24,byte3);
	}
	return result;
}		


CORE_UINT(TAGBITS) Cache::getTag(CORE_UINT(32) address){
	return address.SLC(TAGBITS,3);
}	

CORE_UINT(SETBITS) Cache::getSet(CORE_UINT(32) address){
	return address.SLC(SETBITS,2+SETBITS);
}

CORE_UINT(2) Cache::getOffset(CORE_UINT(32) address){
	return address.SLC(2,0);
}
