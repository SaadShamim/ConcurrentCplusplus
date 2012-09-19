#ifndef _TRUCK_H_
#define _TRUCK_H_

#include <uC++.h>

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;
_Task VendingMachine;

_Task Truck {
	Printer *prt;
	NameServer * nameServer;
	BottlingPlant * plant;
	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;
	unsigned int * cargo;
	VendingMachine ** vms;
	void main();
	public:
	Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
			unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
	~Truck();

};

#endif

