#include "q1buffer.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

void Producer::main(){

	unsigned int rand_seed = getpid();

	for (int i=1; i<Produce+1; i++){
		yield(rand_r(&rand_seed) % Delay); //yield
		buffer->insert(i); //insert
	}	
}



