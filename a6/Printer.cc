#include <uC++.h>
#include <string.h>
#include <iostream>
#include "Printer.h"

const char *Printer::headerDivider = "******"; 
// note B occurs twice, and S prints two values only for students,
//  and R prints value only for NemServer. P prints 1 value only for truck
// so these special cases must be dealt with separately.
const char *Printer::statesNeedingOneValue = "RGBVPS";
const char *Printer::statesNeedingTwoValues = "RDCNTtBSdU";

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, 
			unsigned int numCouriers ) : numStudents(numStudents), 
			numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
	assert((int) Student == 5);
	unsigned int numAgents = (int) Student + numStudents + numVendingMachines + numCouriers;
	states = new char[numAgents];
	values1 = new unsigned int[numAgents];
	values2 = new unsigned int[numAgents];
	for (unsigned int i = 0; i < numAgents; i++) {
		states[i] = 0;
		values1[i] = 0;
		values2[i] = 0;
	}
	printHeader();
}

Printer::~Printer() {
	delete[] states;
	delete[] values1;
	delete[] values2;
	std::cout << "***********************" << std::endl;
}

// For print calls which pass in an id number, the index in the status and values array
// must be calculated from the Kind and id. Thus, this function returns the appropriate
// column number to be used in those arrays
unsigned int Printer::kindAndLidToColumn(Kind kind, unsigned int lid) {
	if (kind == Student) {
		return ((int) Student) + lid;
	} else if (kind == Vending) {
		return ((int) Student) + numStudents + lid;
	} else if (kind == Courier) {
		return ((int) Student) + numStudents + numVendingMachines + lid;
	}
	// should never get here
	assert(false);
	return 0;
}

void Printer::print( Kind kind, char state ) {
	if (state == 'F') {
		printFinishedLine( (int) kind );
		return;
	}
	if (states[(int) kind] != 0) {
		resetAllAndPrint();
	}
	states[(int) kind] = state;
}

void Printer::print( Kind kind, char state, int value1 ) {
	if (state == 'F') {
		printFinishedLine( (int) kind );
		return;
	}
	if (states[(int) kind] != 0) {
		resetAllAndPrint();
	}
	states[(int) kind] = state;
	values1[(int) kind] = value1;
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
	if (state == 'F') {
		printFinishedLine( (int) kind );
		return;
	}
	if (states[(int) kind] != 0) {
		resetAllAndPrint();
	}
	states[(int) kind] = state;
	values1[(int) kind] = value1;
	values2[(int) kind] = value2;
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
	unsigned int col = kindAndLidToColumn(kind, lid);
	if (state == 'F') {
		printFinishedLine( col );
		return;
	}
	if (states[col] != 0) {
		resetAllAndPrint();
	}
	states[col] = state;
}

void Printer::print( Kind kind, unsigned int lid, char state, 
			unsigned int v1) {
	unsigned int col = kindAndLidToColumn(kind, lid);
	if (states[col] != 0) {
		resetAllAndPrint();
	}
	states[col] = state;
	values1[col] = v1;
}

void Printer::print( Kind kind, unsigned int lid, char state, 
		unsigned int v1, unsigned int v2 ) {
	unsigned int col = kindAndLidToColumn(kind, lid);
	if (states[col] != 0) {
		resetAllAndPrint();
	}
	states[col] = state;
	values1[col] = v1;
	values2[col] = v2;
}

void Printer::printFinishedLine( unsigned int col ) {
	// check if there is any data
	unsigned int numAgents = (int) Student + numStudents + 
		numVendingMachines + numCouriers;
	for (unsigned int i = 0; i < numAgents; i++) {
		if (states[i] != 0) {
			resetAllAndPrint();
			break;
		}
	}
	unsigned int i = 0;
	for ( ; i < col; i++) {
		std::cout << "..." << '\t';
	}
	std::cout << "F";
	i += 1;
	for ( ; i < numAgents; i++) {
		std::cout << "\t...";
	}
	std::cout << std::endl;
}

void Printer::resetAllAndPrint() {
	unsigned int numAgents = (int) Student + numStudents + numVendingMachines + numCouriers;
	unsigned int numProcessedColumns = 0;
	for (unsigned int i = 0; i < numAgents; i++) {
		if ( states[i] == 0) {
			numProcessedColumns += 1;
		} else {
			// print out the tab characters for the previous columns
			for (unsigned int k = 0; k < numProcessedColumns; k++) {
				std::cout << '\t';
			}
			numProcessedColumns = 0;
			// print out the state
			std::cout << (char) states[i];
			if( strchr(statesNeedingOneValue, states[i]) != NULL && strchr(statesNeedingTwoValues, states[i]) != NULL ) {
				// character falls in both the "needing one value" and "needing two values" sets
				if(states[i] == 'S') {
					if( (int) Student <= i && (int) Student + numStudents > i) {
						// student and start state. need to print two values
						std::cout << values1[i];
						std::cout << "," << values2[i];
					} else if ( (int) Student + numStudents <= i && (int) Student + numStudents + numVendingMachines > i) {
						// vending machine starte state. Print 1 value
						std::cout << values1[i];
					}
				} else if (states[i] == 'B') {
					if(i >= (int) Student && (int) Student + numStudents > i) {
						// state B and student, so print one value
						std::cout << values1[i];
					} else {
						// not a student, print two values for state B
						std::cout << values1[i];
						std::cout << "," << values2[i];
					}
				} else if (states[i] == 'R') {
					if(i == (int) NameServer) {
						std::cout << values1[i];
					}
				}
			} else if( strchr(statesNeedingOneValue, states[i]) != NULL) {
				if ( states[i] == 'P' ) {
					// only print value with P if is a truck
					if ( i == (int) Truck ) {
						std::cout << values1[i];
					}
				} else {
					std::cout << values1[i];
				}
			} else if( strchr(statesNeedingTwoValues, states[i]) != NULL) {
				std::cout << values1[i];
				std::cout << "," << values2[i];
			}
			numProcessedColumns += 1;
		}
		states[i] = 0;
		values1[i] = 0;
		values2[i] = 0;
	}
	std::cout << std::endl;

}

void Printer::printHeader() {
	std::cout << "Parent\tWATOff\tNames\tTruck\tPlant";
	for (unsigned int i = 0; i < numStudents; i++) {
		std::cout << "\tStud" << i;
	}
	for (unsigned int i = 0; i < numVendingMachines; i++) {
		std::cout << "\tMach" << i;
	}
	for (unsigned int i = 0; i < numCouriers; i++) {
		std::cout << "\tCour" << i;
	}
	std::cout << std::endl;
	unsigned int numAgents = Student + numStudents + numVendingMachines + numCouriers;
	for (unsigned int i = 0; i < numAgents - 1; i++) {
		std::cout << headerDivider << '\t';
	}
	std::cout << headerDivider << std::endl;
}
