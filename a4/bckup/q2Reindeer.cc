#include "q2Reindeer.h"
#include "q2Workshop.h"
#include "q2PRNG.h"
#include "q2Printer.h"

#include <iostream>
using namespace std;

extern PRNG myPrng;

void Reindeer::main(){
	
	//cerr << "Elf Created: " << id << endl;
	yield(myPrng(10));
	printer->print(id, Printer::Starting);

	for(int i=0; i<numDeliveries; i++){
		yield(myPrng(3));
	
		printer->print(id, Printer::OnVacation);
		//cerr << "Elf: Working" << endl;
	
		yield(myPrng(5));
	
		//cerr << "Elf: Help" << endl;
		printer->print(id, Printer::CheckingIn);
	
		wrk->deliver(id);
		
		printer->print(id, Printer::DeliveringToys);
		yield(myPrng(5));
	
		wrk->doneDelivering(id);
	}
	
	printer->print(id, Printer::Finished);
	
}

Reindeer::Reindeer( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numDeliveries ):id(id), wrk(&wrk),printer(&prt), numDeliveries(numDeliveries){}
