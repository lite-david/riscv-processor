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
	for(i=0;i<1000;i++){
		value = rand() % 120;
		dram.setMemory(i,value);
	}
	cout << dram.getMemory(0,0);
	
	return 0;
}
