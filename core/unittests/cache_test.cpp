// vim: set ts=4 nu ai:
#include <iostream>
#include <dram.h>
#include <cache.h>
#include <stdlib.h>

using namespace std;

void printStats(Cache* cache){
	cout << "Cache Misses: " << cache->getNumberCacheMiss() << endl;
	cout << "DRAM reads  : " << cache->getNumberDramReads() << endl;
	cout << "DRAM writes : " << cache->getNumberDramWrites() << endl;
	cout << "Loads       : " << cache->getNumberLoads() << endl;
	cout << "Stores      : " << cache->getNumberStores() << endl;
}

bool load_test(int address, Cache* cache, Dram* dram){
	CORE_UINT(1) dummy;
	return cache->load(address,0,0,&dummy) == dram->getMemory(address);
}

void store_test(int address, Cache* cache, Dram* dram){
	CORE_UINT(1) dummy;
	cache->store(address,46,0,&dummy);
}

int main(){
	
	Dram dram;
	Cache cache(&dram);
	int i=0,value;	
	CORE_UINT(1) dummy;
	//fill the dram
	for(i=0;i<12000;i++){
		value = rand() % 120;
		dram.setMemory(i,value);
	}
	
	int address[5] = {8,4000,100,2000,5000};

	for(i = 0;i<5;i++){
		if(!load_test(address[i],&cache,&dram)){
			cout << "Test " << i << "failed!" << endl;
		}
	}

	cache.store(8,10,0,&dummy);
	cout << cache.load(8,0,0,&dummy) << endl;
	cache.store(4000,22,0,&dummy);
	cout << cache.load(4000,0,0,&dummy) << endl;

	printStats(&cache);
	return 0;
}
