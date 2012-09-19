#ifndef _STUDENT_H_
#define _STUDENT_H_
#include <uC++.h>

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;



_Task Student {
 
	//variables
	Printer *printer;
	NameServer *nameServer;
	WATCardOffice *cardOffice;
	unsigned int id;
	unsigned int maxPurchases;

	//functions
	void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases );

};

#endif
