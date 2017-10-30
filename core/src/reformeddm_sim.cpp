/* vim: set ts=4 ai nu: */
#include <lib/elfFile.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <core.h>
#include <portability.h>
#include <vector>
#include <dram.h>
//#include "sds_lib.h"

#ifdef __VIVADO__
	#include "DataMemory.h"
#endif

using namespace std;

class Simulator{

	private:
		//counters
		Dram dram;
		CORE_UINT(32) nbcycle;
		CORE_UINT(32) ins_addr;
		CORE_UINT(32) pc;

	public:
		CORE_INT(32)* icache;
		CORE_INT(32)* dcache;

		Simulator(){
			icache = (CORE_INT(32) *)malloc(256 * sizeof(CORE_INT(32)));
			dcache = (CORE_INT(32) *)malloc(256 * sizeof(CORE_INT(32)));
			for(int i =0;i<256;i++){
				icache[i]=0;
				dcache[i]=0;
			}
		}

		~Simulator(){
			free(icache);
			free(dcache);
		}

		void fillMemory(){
			//Check whether data memory and instruction memory from program will actually fit in processor.

			//fill instruction memory
			for(map<CORE_UINT(32), CORE_UINT(8) >::iterator it = ins_memorymap.begin(); it!=ins_memorymap.end(); ++it){
				ins_memory[(it->first/4) % 8192].SET_SLC(((it->first)%4)*8,it->second);
			}

			//fill data memory
			for(map<CORE_UINT(32), CORE_UINT(8) >::iterator it = data_memorymap.begin(); it!=data_memorymap.end(); ++it){
				//data_memory.set_byte((it->first/4)%8192,it->second,it->first%4);
				data_memory[(it->first%8192)/4].SET_SLC(((it->first%8192)%4)*8,it->second);
			}
		}

		void setInstructionMemory(CORE_UINT(32) addr, int byte, CORE_INT(8) value){
			if(ins_memorymap[addr].size() == 0){
				ins_memorymap[addr] = vector<int>(4);
			}
			ins_memorymap[addr][byte] = value;
		}

		void setDataMemory(CORE_UINT(32) addr, int byte, CORE_INT(8) value){
			if(data_memorymap[addr].size() == 0){
		   		data_memorymap[addr] = vector<int>(4);
			}
			data_memorymap[addr][byte] = value;
		}

		CORE_INT(32)* getICache(){
			return icache;
		}


		CORE_INT(32)* getDCache(){
			return dcache;
		}

		void setPC(CORE_UINT(32) pc){
			this->pc = pc;
		}

		CORE_UINT(32) getPC(){
			return pc;
		}

};


int main(){
	char* binaryFile = "benchmarks/build/matmul4_4.out";
	ElfFile elfFile(binaryFile);
	Simulator sim;
	int counter = 0;
	for (unsigned int sectionCounter = 0;sectionCounter<elfFile.sectionTable->size(); sectionCounter++){
        ElfSection *oneSection = elfFile.sectionTable->at(sectionCounter);
        if(oneSection->address != 0 && oneSection->getName().compare(".text")){
            //If the address is not null we place its content into memory
            unsigned char* sectionContent = oneSection->getSectionCode();
            for (unsigned int byteNumber = 0;byteNumber<oneSection->size; byteNumber++){
            	counter++;
                sim.setDataMemory(oneSection->address, byteNumber, sectionContent[byteNumber]);
            }
        }

        if (!oneSection->getName().compare(".text")){
        	unsigned char* sectionContent = oneSection->getSectionCode();
            for (unsigned int byteNumber = 0;byteNumber<oneSection->size; byteNumber++){
                sim.setInstructionMemory(oneSection->address, byteNumber, sectionContent[byteNumber]);
            }
    	}
    }

    for (int oneSymbol = 0; oneSymbol < elfFile.symbols->size(); oneSymbol++){
		ElfSymbol *symbol = elfFile.symbols->at(oneSymbol);
		const char* name = (const char*) &(elfFile.sectionTable->at(elfFile.indexOfSymbolNameSection)->getSectionCode()[symbol->name]);
		if (strcmp(name, "_start") == 0){
			fprintf(stderr, "%s\n", name);
			sim.setPC(symbol->offset);
		}
	}


    sim.fillMemory();
//    CORE_INT(32)* dm_in;
//    dm_in = sim.getDataMemory();
    dm_out = (CORE_INT(32) *)malloc(8192 * sizeof(CORE_INT(32)));
    int ins;
    std::cin >> ins;
    doStep(sim.getPC(),ins,sim.getInstructionMemory(),sim.getDataMemory(),dm_out);
    /*for(int i = 0;i<34;i++){
    	std::cout << std::dec << i << " : ";
    	std::cout << std::hex << debug_out[i] << std::endl;
    }*/
    //std::cout << "dm" <<std::endl;
    //for(int i = 0;i<8192;i++){
    //	   	std::cout << std::dec << i << " : ";
    //    	std::cout << std::dec << dm_out[i] << std::endl;
    // }
    free(dm_out);
	return 0;
}
