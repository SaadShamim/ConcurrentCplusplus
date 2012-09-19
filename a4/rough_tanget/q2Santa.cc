#include "q2Santa.h"
#include "q2Workshop.h"
#include "q2PRNG.h"
#include "q2Printer.h"

#include <iostream>
using namespace std;

extern PRNG myPrng;

void Santa::main(){
	//start
	yield(myPrng(10));
	printer->print(0, Printer::Starting);
	
	while(true){
		
		yield(myPrng(3));
		
		printer->print(0, Printer::Napping);
		//go to monitor to Nap
		Workshop::Status x = wrk->sleep();
		
		printer->print(0, Printer::Awake);
		
		//determine which task to complete
		if(x == Workshop::Delivery){
			printer->print(0, Printer::DeliveringToys);
			yield(myPrng(5));
			wrk->doneDelivering(0); //start delivery
		} else if( x == Workshop::Consulting) {
			printer->print(0, Printer::Consulting);
			yield(myPrng(3));
			wrk->doneConsulting(0); //start consultation
		} else {
			break; //end of program
		}
	
	
	}
	
	//print finishing message
	printer->print(0, Printer::Finished);

}

//constructor
Santa::Santa( Workshop &wrk, Printer &prt):wrk(&wrk),printer(&prt){}


