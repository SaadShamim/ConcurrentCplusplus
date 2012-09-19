#ifndef _PRINTER_H_
#define _PRINTER_H_
#include <uC++.h>

_Monitor Printer {                  // choose monitor or cormonitor
  public:
	enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, 
		Student, Vending, Courier };
	Printer( unsigned int numStudents, unsigned int numVendingMachines, 
			unsigned int numCouriers );
	~Printer();
	void print( Kind kind, char state );
	void print( Kind kind, char state, int value1 );
	void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, 
			unsigned int v1);
    void print( Kind kind, unsigned int lid, char state, 
			unsigned int v1, unsigned int v2 );
  private:
	const static char * headerDivider; 
	const static char * statesNeedingTwoValues;
	const static char * statesNeedingOneValue;
	const unsigned int numStudents, numVendingMachines, numCouriers;
	void resetAllAndPrint();
	void printHeader();
	char * states;			// array of states
	unsigned int * values1;
	unsigned int * values2;
	void printFinishedLine( unsigned int col );
	unsigned int kindAndLidToColumn(Kind kind, unsigned int lid);
};

#endif
