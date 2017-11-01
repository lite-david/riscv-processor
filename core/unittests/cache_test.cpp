#include <iostream>
#include <dram.h>
#include <cache.h>
#include <stdlib.h>

using namespace std;

int main(){
	
	Dram dram;
	Cache cache(&dram);
	int i=0,value;	
	//fill the dram
	for(i=0;i<120000;i++){
		value = rand() % 120;
		dram.setMemory(i,value);
	}
	cout << dram.getMemory(25,0) << endl;
	cout << dram.getMemory(200,0) << endl;
	cout << cache.load(100,0,0) << endl;
	cache.store(300,17,0);
	cache.store(4,88,0);
	cout << cache.load(300,0,0) << endl;
	cout << cache.load(4,0,0) << endl;
	cout << cache.load(1000,0,0) << endl;
	cout << dram.getMemory(75,0) << endl;
	cout << dram.getMemory(1,0) << endl;
	return 0;
}
