#ifndef _Q2_PRINTER
#define _Q2_PRINTER
#include <uC++.h>
#include "q2Philosopher.h"

_Monitor Printer {                  // choose monitor or cormonitor
 
	int *bufferArray;
	unsigned int *leftValue;
	unsigned int *rightValue;

	unsigned int numberOfPhil;
	
	void clearBuffer();
	void print_tabs(int tabs);
  public:
    Printer( unsigned int NoOfPhil );
    void print( unsigned int id, Philosopher::States state );
    void print( unsigned int id, Philosopher::States state, unsigned int bite, unsigned int noodles );
    ~Printer();
};
#endif
