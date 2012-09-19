#include "q2Reindeer.h"
#include "q2Workshop.h"
#include "q2PRNG.h"
#include "q2Printer.h"

#include <iostream>
using namespace std;

extern PRNG myPrng;

void Reindeer::main(){
	
	//start
	yield(myPrng(10));
	printer->print(id, Printer::Starting);

	//loop until max number of deliveries have been made
	for(int i=0; i<numDeliveries; i++){
		yield(myPrng(3));
	
		printer->print(id, Printer::OnVacation);
	
		yield(myPrng(5)); //simulate vacation
	
		printer->print(id, Printer::CheckingIn);
		
		//request santa for deliver
		wrk->deliver(id);
		
		printer->print(id, Printer::DeliveringToys);
		yield(myPrng(5)); //simulate delivery
	
		//wait for all reindeers to finish delivery
		wrk->doneDelivering(id);
	}
	
	//finishing message
	printer->print(id, Printer::Finished);
	
}

//constructor
Reindeer::Reindeer( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numDeliveries ):id(id), wrk(&wrk),printer(&prt), numDeliveries(numDeliveries){}
