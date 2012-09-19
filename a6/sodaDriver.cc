// Purpose: Run vending machine simulation with Watcards, watcard office, soda plant, etc.
//
// Command syntax:
//   $ ./soda
//

#include <uC++.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;					// direct access to std
#include <cstdlib>					// exit
#include "Printer.h"
#include "Bank.h"
#include "Parent.h"
#include "WATCardOffice.h"
#include "NameServer.h"
#include "VendingMachine.h"
#include "BottlingPlant.h"
#include "Student.h"
#include "MPRNG.h"
#include "Config.h"
#include <string>

MPRNG mprng;

bool convert( int &val, char *buffer ) {		// convert C string to integer
    std::stringstream ss( buffer );			// connect stream and buffer
    ss >> dec >> val;					// convert integer from buffer
    return ! ss.fail() &&				// conversion successful ?
	// characters after conversion all blank ?
	string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
} // convert

void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0]
    << " [config-file [ random-seed > 0 ] ] " << std::endl;
    exit( EXIT_FAILURE );				// TERMINATE
} // usage

void uMain::main() {
	//default filename and seed
	string filename = "soda.config";
	int randSeed = getpid();

    switch ( argc ) {
        case 3: //get seed value
            if ( ! convert( randSeed, argv[2] ) || randSeed < 1 ) {	// invalid integer ?
    	        usage( argv );
    	    } // if
            // FALL THROUGH
        case 2: //get file name
			filename = argv[1];
            // FALL THROUGH
        default:  
            break;
    } // switch

	mprng.seed(randSeed);

	ConfigParms params;
	//process given fil (or default)
	processConfigFile(filename.c_str(), params);

	//start all classes in appropriate order	
	Printer printer(params.numStudents, params.numVendingMachines, params.numCouriers);
	Bank bank(params.numStudents);
	Parent parent(printer, bank, params.numStudents, params.parentalDelay);
	WATCardOffice office(printer, bank, params.numCouriers);
	NameServer nameServer(printer, params.numVendingMachines, params.numStudents);
	VendingMachine ** vms = new VendingMachine * [params.numVendingMachines];
	for (unsigned int i = 0; i < params.numVendingMachines; i++) {
		vms[i] = new VendingMachine(printer, nameServer, i, params.sodaCost, params.maxStockPerFlavour);
	}
	BottlingPlant * plant = new BottlingPlant (printer, nameServer, params.numVendingMachines, params.maxShippedPerFlavour, params.maxStockPerFlavour, params.timeBetweenShipments);
	Student ** students = new Student * [params.numStudents];
	for (unsigned int i = 0; i < params.numStudents; i++) {
		students[i] = new Student(printer, nameServer, office, i, params.maxPurchases);
	}

	//begin to delete all dynamically allocated objects
	for (unsigned int i = 0; i < params.numStudents; i++) {
		delete students[i];
	}
	delete[] students;
	delete plant;
	for (unsigned int i = 0; i < params.numVendingMachines; i++) {
		delete vms[i];
	}
	delete[] vms;
} // uMain::main

// Local Variables: //
// compile-command: "u++ uIO.cc" //
// End: //
