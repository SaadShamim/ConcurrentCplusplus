#include "q2Elf.h"
#include "q2Workshop.h"
#include "q2PRNG.h"
#include "q2Printer.h"

#include <iostream>
using namespace std;

extern PRNG myPrng;

void Elf::main(){
	
	//cerr << "Elf Created: " << id << endl;
	yield(myPrng(10));
	printer->print(id, Printer::Starting);

	for(int i=0; i<numConsultations; i++){
		yield(myPrng(3));
	
		printer->print(id, Printer::Working);
		//cerr << "Elf: Working" << endl;
	
		yield(myPrng(5));
	
		//cerr << "Elf: Help" << endl;
		printer->print(id, Printer::NeedHelp);
	
		bool status = wrk->consult(id);
	
		if(!status){
			printer->print(id, Printer::ConsultingFailed);
			break;
		}
		
		printer->print(id, Printer::Consulting);
		yield(myPrng(3));
	
		wrk->doneConsulting(id);
	}
	
	wrk->termination(id);
	
	printer->print(id, Printer::Finished);
	
}

Elf::Elf( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numConsultations ):id(id), wrk(&wrk),printer(&prt), numConsultations(numConsultations){}
