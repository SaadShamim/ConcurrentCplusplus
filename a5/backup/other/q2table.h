#ifndef _Q2_TABLE
#define _Q2_TABLE

#if defined( TABLETYPE_INT )                    // internal scheduling monitor solution
// includes for this kind of table
class Printer;
#include <uC++.h>

_Monitor Table {

	
	bool *eatArray;
	uCondition *waitingToPickup;
	int *waitingToEat;
	int *amountEaten;
	int *noodlesLeft;
	int debug;
	

	int wakeNext;
	
    // private declarations for this kind of table
#elif defined( TABLETYPE_AUTO )                 // automatic-signal monitor solution

class Printer;
#include <uC++.h>
#include "AutomaticSignal.h"
// includes for this kind of table
_Monitor Table {
	AUTOMATIC_SIGNAL;
	bool *eatArray;
	
	
	
    // private declarations for this kind of table
#elif defined( TABLETYPE_TASK )                 // internal/external scheduling task solution
#include <uC++.h>
class Printer;
	
_Task Table {
	bool *eatArray;
	
	int globalId;
	uCondition *waitingToPickup;
	bool debug;
	
	void main();
    // private declarations for this kind of table
#else
    #error unsupported table
#endif
	unsigned int numberOfPhil;
	Printer *printer;
	
	int leftFork(unsigned int id);
	int rightFork(unsigned int id);
    // common declarations
  public:                                       // common interface
    Table( unsigned int noOfPhil, Printer &prt );
    ~Table();
    void pickup( unsigned int id );
    void putdown( unsigned int id );
};

#endif
