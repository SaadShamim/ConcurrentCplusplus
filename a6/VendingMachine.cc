#include <uC++.h>
#include "VendingMachine.h"
#include "NameServer.h"
#include "WATCard.h"
#include "Printer.h"

VendingMachine::Status VendingMachine::buy(Flavours flavour, WATCard &card){
	if(stock[flavour] > 0 && card.getBalance() >= sodaCost){ //valid stock and funds, buy
		stock[flavour] -=1;
		assert(stock[flavour] >= 0);
		card.withdraw(sodaCost);
		printer->print(Printer::Vending, id, 'B',(int)flavour,(int)stock[flavour]);
		return VendingMachine::BUY;
	}else if(stock[flavour] == 0){ //not enough stock
		return VendingMachine::STOCK;
	}else if(card.getBalance() < sodaCost){ //not enough fund in watcard
		return VendingMachine::FUNDS;
	}

	//this should never run, implies missed case
	assert(false);
}

//returns stock
unsigned int *VendingMachine::inventory(){
	printer->print(Printer::Vending, id, 'r');
	restocking = true;
	return stock;
}

//when stock has been restocked this function is called
void VendingMachine::restocked(){
	assert(restocking == true);
	restocking = false;
	printer->print(Printer::Vending, id, 'R');
}
	

unsigned int VendingMachine::cost(){
	return sodaCost;
}

unsigned int VendingMachine::getId(){
	return id;
}

void VendingMachine::main(){
	for( ; ;){
		_Accept(~VendingMachine){
			break;
		} or _Accept(inventory){
		} or _Accept(restocked){
		} or _When(restocking == false) _Accept(buy){
		}
	}
	printer->print(Printer::Vending, id, 'F');
	
}

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour ):printer(&prt),nameServer(&nameServer),id(id),sodaCost(sodaCost),maxStockPerFlavour(maxStockPerFlavour),stock(new unsigned int[4]) {
	printer->print(Printer::Vending, id, 'S',sodaCost);
	restocking = false;
	nameServer.VMregister(this);
	//initialize all four flavour's stock to 0
	for(int i=0;i<4;i++){
		stock[i]=0;	
	}
}


VendingMachine::~VendingMachine() {
	delete[] stock;
}
