// vim: set ts=4 nu ai:
#include <cache.h>
#include <assert.h>
#include <iostream>

Cache::Cache(Dram* dram){
	assert(SETBITS + TAGBITS + 2 == 32);
	assert(1 << SETBITS == SETS);
	dram_location = dram;
	n_cache_miss = 0;
	n_load = 0;
	n_store = 0;
	n_dram_writes = 0;
	n_dram_reads = 0;
	int i =0;
	for(i=0;i<SETS;i++){
		cache[i][0] = 0;
		cache[i][1] = 0;
		cache[i][2] = 0;		
		cache[i][3] = 0;
		index[i].tag = 0;
		index[i].set = 0;
		index[i].dirtybit=0;
	}
}

void Cache::store(CORE_UINT(32) address, CORE_INT(32) value, CORE_UINT(2) op, CORE_UINT(1)* cache_miss){
	// For store byte, op = 0
	// For store half word, op = 1
	// For store word, op = 3
	
	n_store++;
	CORE_UINT(TAGBITS) tag = getTag(address);
	CORE_UINT(SETBITS) set = getSet(address);
	CORE_UINT(2) offset = getOffset(address);
	CORE_UINT(8) byte0 = value.SLC(8,0);
	CORE_UINT(8) byte1 = value.SLC(8,8);
	CORE_UINT(8) byte2 = value.SLC(8,16);
	CORE_UINT(8) byte3 = value.SLC(8,24);

	CORE_UINT(32) dram_address = 0;
	dram_address.SET_SLC(0,set);
	dram_address.SET_SLC(SETBITS,index[set].tag);
	CORE_UINT(1) tag_match = (tag == index[set].tag) ? 1 : 0;
	CORE_UINT(1) dirty = index[set].dirtybit;
	if(dirty && tag_match == 0){
		n_cache_miss++;
		//*cache_miss = 1;
		n_dram_writes++;
		dram_location->setMemory(dram_address,cache[set][0],0);
		dram_location->setMemory(dram_address,cache[set][1],1);
		dram_location->setMemory(dram_address,cache[set][2],2);
		dram_location->setMemory(dram_address,cache[set][3],3);
	}

	cache[set][offset] = byte0;
	if(op & 1){
		cache[set][offset+1] = byte1;
	}
	if(op & 2){
		cache[set][2] = byte2;
		cache[set][3] = byte3;
	}
	index[set].set = set;
	index[set].tag = tag;
	index[set].dirtybit = 1;
}

CORE_INT(32) Cache::load(CORE_UINT(32) address, CORE_UINT(2) op, CORE_UINT(1) sign, CORE_UINT(1)* cache_miss){
	// For load byte, op = 0
	// For load half word, op = 1
	// For load word, op = 3
	n_load++;
	CORE_UINT(SETBITS+1) i;	
	CORE_UINT(TAGBITS) tag = getTag(address);
	CORE_UINT(SETBITS) set = getSet(address);
	CORE_UINT(2) offset = getOffset(address);
	CORE_UINT(1) tag_match = (tag == index[set].tag) ? 1 : 0;
	CORE_UINT(1) set_match = (set == index[set].set) ? 1 : 0;
	CORE_UINT(1) dirty = index[set].dirtybit;
	CORE_INT(32) result;
	result = sign ? -1 : 0;
	CORE_UINT(32) dram_address = 0;
	CORE_UINT(8) byte0, byte1, byte2, byte3;
	if(tag_match & set_match == 1){
		byte0 = cache[set][0];
		byte1 = cache[set][1];
		byte2 = cache[set][2];
		byte3 = cache[set][3];
	}
	else{
		n_dram_reads++;
		n_cache_miss++;
		//*cache_miss = 1;
		for(i = 0; i< SETS;i++){
			if(index[i].dirtybit == 1){
				n_dram_writes++;
				break;
			}
		}
		//write all dirty blocks to memory
		for(i = 0;i<SETS;i++){
			if(index[i].dirtybit == 1){
				dram_address = 0;
				dram_address.SET_SLC(0,i);
				dram_address.SET_SLC(SETBITS,index[i].tag);
				dram_location->setMemory(dram_address, cache[i][0], 0);
				dram_location->setMemory(dram_address, cache[i][1], 1);
				dram_location->setMemory(dram_address, cache[i][2], 2);
				dram_location->setMemory(dram_address, cache[i][3], 3);
				index[i].dirtybit = 0;
			}
		}

		dram_address = 0;
		dram_address.SET_SLC(SETBITS,tag);
		//reading blocks into cache
		for(i = 0; i< SETS; i++){
				cache[i][0] = dram_location->getMemory(dram_address,0);
				cache[i][1] = dram_location->getMemory(dram_address,1);
				cache[i][2] = dram_location->getMemory(dram_address,2);
				cache[i][3] = dram_location->getMemory(dram_address,3);
				index[i].set = i;
				index[i].tag = getTag(dram_address << 2);
				dram_address+=1;
		}

		byte0 = cache[set][0];
		byte1 = cache[set][1];
		byte2 = cache[set][2];
		byte3 = cache[set][3];
	}
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
		result.SET_SLC(8,byte1);
	}
	if(op & 2){
		result.SET_SLC(16,byte2);
		result.SET_SLC(24,byte3);
	}
	return result;
}		


CORE_UINT(TAGBITS) Cache::getTag(CORE_UINT(32) address){
	return address.SLC(TAGBITS,2+SETBITS);
}	

CORE_UINT(SETBITS) Cache::getSet(CORE_UINT(32) address){
	return address.SLC(SETBITS,2);
}

CORE_UINT(2) Cache::getOffset(CORE_UINT(32) address){
	return address.SLC(2,0);
}

CORE_UINT(32) Cache::getNumberCacheMiss(){
	return n_cache_miss;
}


CORE_UINT(32) Cache::getNumberDramReads(){
	return n_dram_reads;
}


CORE_UINT(32) Cache::getNumberDramWrites(){
	return n_dram_writes;
}


CORE_UINT(32) Cache::getNumberLoads(){
	return n_load;
}


CORE_UINT(32) Cache::getNumberStores(){
	return n_store;
}
