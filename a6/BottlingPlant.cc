#include <uC++.h>
#include "BottlingPlant.h"
#include "Truck.h"
#include "Printer.h"
#include "MPRNG.h"
#include "VendingMachine.h"
extern MPRNG mprng;

//Initialize values, create truck;
BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments ) :
			prt(&prt), nameServer(&nameServer), 
			numVendingMachines(numVendingMachines),
			maxShippedPerFlavour(maxShippedPerFlavour),
			maxStockPerFlavour(maxStockPerFlavour),
			timeBetweenShipments(timeBetweenShipments),
			shipmentReady(false), shuttingDown(false) {
	truck = new Truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);
	currentProd = new unsigned int[VendingMachine::FLAVOURS_COUNT];
	notifiedTruck = false;
}

void BottlingPlant::main() {
	prt->print(Printer::BottlingPlant, 'S'); 
	int totalProduced = 0;
	while(true) {
		totalProduced = 0;
		assert(shipmentReady == false);
		yield(timeBetweenShipments);
		// produce the soda
		for (unsigned int i = 0; i < VendingMachine::FLAVOURS_COUNT; i++) {
			currentProd[i] = mprng(maxShippedPerFlavour);
			totalProduced += currentProd[i];
		}
		prt->print(Printer::BottlingPlant, 'G',totalProduced); 
		//shipment is ready
		shipmentReady = true;
		_Accept(getShipment) {
			prt->print(Printer::BottlingPlant, 'P'); 
		} or _Accept(~BottlingPlant) {
			shuttingDown = true;
			notifiedTruck = false;
			//don't close until the truck is notified that plant is closing
			while(!notifiedTruck) {
				// wake up all calls to getShipment on the entry queue
				_Accept(getShipment) {

				} 
			}
			break;
		}
	}
	prt->print(Printer::BottlingPlant, 'F'); 
}
//set the trucks cargo[] to the current prod
bool BottlingPlant::getShipment( unsigned int cargo[] ) {
	if(shuttingDown) {
		notifiedTruck = true;
		return true;
	}
	assert(shipmentReady);
	//give prod to truck's cargo
	for (unsigned int i = 0; i < VendingMachine::FLAVOURS_COUNT; i++) {
		cargo[i] = currentProd[i];
	}
	shipmentReady = false;
	return false;
}

BottlingPlant::~BottlingPlant() {
	delete truck;
	delete[] currentProd;
}
