#include "q2Elf.h"
#include "q2Workshop.h"
#include "q2PRNG.h"
#include "q2Printer.h"

#include <iostream>
using namespace std;

extern PRNG myPrng;

void Elf::main(){
	
	//start
	yield(myPrng(10));
	printer->print(id, Printer::Starting);
	
	//upper bound set at number of consultation by each elf
	for(int i=0; i<numConsultations; i++){
		yield(myPrng(3));
	
		printer->print(id, Printer::Working);
		
	
		yield(myPrng(5)); //simlate work
	
		printer->print(id, Printer::NeedHelp); //print H
	
		//call monitor to request consultation from santa
		bool status = wrk->consult(id);
	
		//if consultation not granted, break out of loop and terminate
		if(!status){
			printer->print(id, Printer::ConsultingFailed);
			break;
		}
		
		//simulate consultation
		printer->print(id, Printer::Consulting);
		yield(myPrng(3));
		
		//finish consultation
		wrk->doneConsulting(id);
	}
	
	//terminate elf
	wrk->termination(id);
	
	//print finishing message
	printer->print(id, Printer::Finished);
	
}

//constructor initialize values
Elf::Elf( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numConsultations ):id(id), wrk(&wrk),printer(&prt), numConsultations(numConsultations){}
