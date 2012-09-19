#include "q2Santa.h"
#include "q2Workshop.h"
#include "q2PRNG.h"
#include "q2Printer.h"

#include <iostream>
using namespace std;

extern PRNG myPrng;

void Santa::main(){
	yield(myPrng(10));
	printer->print(0, Printer::Starting);
	//cerr << "Santa Starting" << endl;
	
	while(true){
		
		yield(myPrng(3));
		
		//cerr << "Napping" << endl;
		printer->print(0, Printer::Napping);
		
		Workshop::Status x = wrk->sleep();
		
		printer->print(0, Printer::Awake);
		
		if(x == Workshop::Delivery){
			printer->print(0, Printer::DeliveringToys);
			yield(myPrng(5));
			wrk->doneDelivering(0);
		} else if( x == Workshop::Consulting) {
			printer->print(0, Printer::Consulting);
			yield(myPrng(3));
			wrk->doneConsulting(0);
		} else {
			break;
		}
	
	
	}
	
	printer->print(0, Printer::Finished);
	//cerr << "Santa: Finished" << endl;

}

Santa::Santa( Workshop &wrk, Printer &prt):wrk(&wrk),printer(&prt){}


