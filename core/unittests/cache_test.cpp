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
	return cache->load(address,0,0) == dram->getMemory(address >> 2, address % 4);
}

void store_test(int address, Cache* cache, Dram* dram){
	cache->store(address,46,0);
}

int main(){
	
	Dram dram;
	Cache cache(&dram);
	int i=0,value;	
	//fill the dram
	for(i=0;i<120000;i++){
		value = rand() % 120;
		dram.setMemory(i,value);
	}
	
	int address[5] = {8,4000,100,2000,5000};

	for(i = 0;i<5;i++){
		if(!load_test(address[i],&cache,&dram)){
			cout << "Test " << i << "failed!" << endl;
		}
	}

	cache.store(8,10,0);
	cout << cache.load(8,0,0) << endl;
	cache.store(4000,22,0);
	cout << cache.load(4004,0,0) << endl;

	printStats(&cache);
	return 0;
}
