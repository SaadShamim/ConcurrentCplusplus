#include <uC++.h>
#include "Student.h"
#include "WATCard.h"
#include "Printer.h"
#include "WATCardOffice.h"
#include "NameServer.h"
#include "VendingMachine.h"
#include "MPRNG.h"

extern MPRNG mprng;

void Student::main(){
	//calculate amount to purchase
	int purchaseAmount = mprng(1,maxPurchases);
	//get flavour of coke
	int flavor = mprng(3);
	//create future wat card
	FWATCard fWatCard = cardOffice->create(id,5);
	WATCard *card;
	printer->print(Printer::Student, id, 'S', flavor, purchaseAmount);	
	
	//get vending machine
	VendingMachine *vendingMachine = nameServer->getMachine(id);
	assert(vendingMachine != NULL);

	printer->print(Printer::Student, id, 'V',vendingMachine->getId());	


	for(int i = 0; i< purchaseAmount; i++){
	
		yield(mprng(1,10));
	
		while(true){

			//try to get future or catch lost event
			while(true) {
				try{
					card = fWatCard();
					break;
				} catch (WATCardOffice::Lost lost) {
					printer->print(Printer::Student, 'L');
					fWatCard = cardOffice->create(id,5);
				}

			}

			//try to buy from vending machine
			VendingMachine::Status status = vendingMachine->buy((VendingMachine::Flavours)flavor, *card);
		
			if(status == VendingMachine::BUY){ //sucessfully bought
				printer->print(Printer::Student, id, 'B', card->getBalance());
				break;
			}else if(status == VendingMachine::STOCK){ //vending machine out of stock
				//request another vending machne
				vendingMachine = nameServer->getMachine(id);
				assert(vendingMachine != NULL);
				printer->print(Printer::Student, id, 'V', vendingMachine->getId());
				yield(mprng(1,10));
			}else if (status == VendingMachine::FUNDS){ //not enough funds for student
				unsigned int canCost = vendingMachine->cost();
				fWatCard = cardOffice->transfer(id,5+canCost, card);
			}else{
				//should never get here, unhandled case
				assert(false);
			}
		}
	
	}

	delete card;
	printer->print(Printer::Student, id, 'F');

}

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases ) : printer(&prt), nameServer(&nameServer), cardOffice(&cardOffice), id(id), maxPurchases(maxPurchases) {
}

