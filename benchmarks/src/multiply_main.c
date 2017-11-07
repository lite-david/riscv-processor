#include "multiply.h"

#include "dataset1.h"

void main(){
	int i;
	int results_data[DATA_SIZE];

	for(i = 0; i< DATA_SIZE;i++){
		results_data[i] = multiply(input_data1[i], input_data2[i]);
	}
}
