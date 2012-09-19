#include <uC++.h>
#include "VendingMachine.h"
#include "NameServer.h"
#include "Printer.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ):printer(&prt),numVendingMachines(numVendingMachines), numStudents(numStudents), vendingMachineArray(new VendingMachine *[numVendingMachines]), studentsMachine(new int[numStudents]){
	vendingMachineIndex = 0;
	for(unsigned int i=0; i<numStudents;i++){
		//the machine each student is assigned
		studentsMachine[i] = -1;
	}
	for(unsigned int i = 0; i < numVendingMachines; i++) {
		//keeps track of all vending machine
		vendingMachineArray[i] = NULL;
	}
}

void NameServer::VMregister(VendingMachine *vendingmachine){
	vendingMachineArray[vendingMachineIndex] = vendingmachine; 
	printer->print(Printer::NameServer, 'R', vendingmachine->getId());
	vendingMachineIndex++;
	assert(vendingMachineIndex <= numVendingMachines);
}

VendingMachine *NameServer::getMachine(unsigned int id){
	VendingMachine * vm = NULL;
	// could be getting a machine number that hasn't registered yet, so loop while null
	while(vm == NULL) {
		if(studentsMachine[id] == -1){
			// At the beginning, the student number id does not have an assignment machine number
			// assign the same number of its id (modulo number of machines)
			studentsMachine[id] = id % numVendingMachines;
		}else{
			// increment the assigned machine number
			studentsMachine[id] = (studentsMachine[id] + 1) % numVendingMachines;
			assert((unsigned int) studentsMachine[id] < numVendingMachines);
		}
		vm = vendingMachineArray[studentsMachine[id]];
	}
	printer->print( Printer::NameServer, 'N', (int) id, (int)  vm->getId());
	return vm;
}

VendingMachine **NameServer::getMachineList(){
	return vendingMachineArray;
}

void NameServer::main(){
	printer->print(Printer::NameServer, 'S');
	for( ; ;){
		// accept deconstructor or calls
		_Accept(~NameServer){
			break;
		}or _Accept(VMregister){
		}or _Accept(getMachine){
		}or _Accept(getMachineList){
		}
	}
	printer->print(Printer::NameServer, 'F');
}

NameServer::~NameServer(){
	delete[] vendingMachineArray;
	delete[] studentsMachine;
}
