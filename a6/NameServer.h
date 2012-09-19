#ifndef _NameServer_H_
#define _NameServer_H_
#include <uC++.h>

_Task VendingMachine;

class Printer;

_Task NameServer {
    	
	Printer *printer;
	unsigned int numVendingMachines;
	unsigned int numStudents;
	VendingMachine **vendingMachineArray;
	unsigned int vendingMachineIndex;
	int *studentsMachine;

	void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
    ~NameServer();
};

#endif
