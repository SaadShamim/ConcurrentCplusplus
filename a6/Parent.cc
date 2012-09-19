#include <uC++.h>
#include "MPRNG.h"
#include "Printer.h"
#include "Bank.h"
#include "Parent.h"

extern MPRNG mprng;

void Parent::main(){
	printer->print(Printer::Parent, 'S');
	for( ; ;){
		_Accept(~Parent){
			break;
		}else{
			int id = mprng(numStudents-1);
			int money = mprng(1,3); //determine amount of money to send
			yield(parentalDelay);
			printer->print(Printer::Parent, 'D', id, money);
			bank->deposit(id,money); //deposit money
		}

	}
	printer->print(Printer::Parent, 'F');
}



Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay):printer(&prt),bank(&bank),numStudents(numStudents), parentalDelay(parentalDelay){}

Parent::~Parent(){}
