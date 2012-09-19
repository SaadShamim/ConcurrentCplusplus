#include "q1buffer.h"
#include <stdlib.h>

void Consumer::main(){

	unsigned int rand_seed = getpid();

	while(true){
		yield(rand_r(&rand_seed) % Delay); //yield 
		int x = buffer->remove(); // call remove
		//if sentinel add to sum
		if(x != Sentinel){
			(*sum) += x;
		}else{
			break;
		}
	}

}
