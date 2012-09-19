#ifndef _BOTTLINGPLANT_H_
#define _BOTTLINGPLANT_H_
#include <uC++.h>

_Monitor Printer;
_Task NameServer;
_Task Truck;

_Task BottlingPlant {
	Printer *prt;
	NameServer *nameServer;
	Truck *truck;
	unsigned int numVendingMachines;
	unsigned int maxShippedPerFlavour;
	unsigned int maxStockPerFlavour;
	unsigned int timeBetweenShipments;
	unsigned int *currentProd;
	bool shipmentReady, shuttingDown;
	bool notifiedTruck;
	void main();
	public:
		BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );

		bool getShipment( unsigned int cargo[] );
		~BottlingPlant();
};

#endif
