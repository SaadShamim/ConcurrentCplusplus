#include <uC++.h>
#include "Truck.h"
#include "NameServer.h"
#include "VendingMachine.h"
#include "Printer.h"
#include "MPRNG.h"
#include "BottlingPlant.h"
extern MPRNG mprng;

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
			unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
			prt(&prt), nameServer(&nameServer), plant(&plant), 
			numVendingMachines(numVendingMachines), 
			maxStockPerFlavour(maxStockPerFlavour) {
	cargo = new unsigned int[VendingMachine::FLAVOURS_COUNT];
	vms = nameServer.getMachineList();
}

void Truck::main() {
	prt->print(Printer::Truck, 'S');	
	bool shuttingDown;
	unsigned int totalNumSodas = 0;
	int unstocked = 0;
	while(true) {
		yield(mprng(1,10));
		//get sodas
		shuttingDown = plant->getShipment(cargo);
		if(shuttingDown) {
			break;
		}
		// print out total number of sodas in shipment
		totalNumSodas = 0;
		for (unsigned int i = 0; i < VendingMachine::FLAVOURS_COUNT; i++) {
			totalNumSodas += cargo[i];
		}
		prt->print(Printer::Truck, 'P', totalNumSodas);	
		// start delivering to the vending machines
		for(unsigned int i = 0; i < numVendingMachines; i++) {
			unstocked = 0;
			totalNumSodas = 0;
			for (unsigned int k = 0; k < VendingMachine::FLAVOURS_COUNT; k++) {
				totalNumSodas += cargo[k];
			}
			if(totalNumSodas == 0) {
				// truck empty. Don't try delivering
				break;
			}
			prt->print(Printer::Truck, 'd', (int) i, (int) totalNumSodas);	
			// Truck not empty, so deliver to this vending machine
			unsigned int *inventory = vms[i]->inventory();
			for (int k = 0; k < VendingMachine::FLAVOURS_COUNT; k++) {
				inventory[k] += cargo[k];
				cargo[k] = 0;
				if (inventory[k] > maxStockPerFlavour) {
					cargo[k] = inventory[k] - maxStockPerFlavour;
					inventory[k] = maxStockPerFlavour;
				}else if(inventory[k] != maxStockPerFlavour){
					unstocked += maxStockPerFlavour - inventory[k];
				}
			}
			if( unstocked != 0 ) {
				prt->print(Printer::Truck, 'U',(int)i,(int) unstocked);	
			}
			//notify vms, that restocking complete
			vms[i]->restocked();
			totalNumSodas = 0;
			for (unsigned int k = 0; k < VendingMachine::FLAVOURS_COUNT; k++) {
				totalNumSodas += cargo[k];
			}
			prt->print(Printer::Truck, 'D',(int)i,(int) totalNumSodas);	
		}
	}
	prt->print(Printer::Truck, 'F');	
}

Truck::~Truck() {
	delete[] cargo;
}
