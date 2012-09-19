#include <uC++.h>
#include "q2Philosopher.h"
//#include "q2table.h"
#include "q2Printer.h"
#include "q2PRNG.h"

extern PRNG myPrng;

void Philosopher::main(){
	while(true){
	
		printer->print(id, Philosopher::Hungry);
		
		yield(myPrng(4));
		
		table->pickup(id);
		
		unsigned int eatAmount = myPrng(1,5);
		
		if(eatAmount > numberOfNoodles)
			eatAmount = numberOfNoodles;
		
		assert(eatAmount < 6);
		numberOfNoodles -= eatAmount;
		
		printer->print(id, Philosopher::Eating, eatAmount, numberOfNoodles);
		
		if(numberOfNoodles == 0) {
			table->putdown(id);
			break;
		}
				
		yield(myPrng(4));
		

		table->putdown(id);
		
		//if(numberOfNoodles == 0)
			//break;
			
		printer->print(id, Philosopher::Thinking);
		
		yield(myPrng(19));
		
	}
	
	printer->print(id, Philosopher::Finished);
	
}

Philosopher::Philosopher( unsigned int id, unsigned int noodles, Table &table, Printer &prt ):id(id),numberOfNoodles(noodles),table(&table),printer(&prt){}
