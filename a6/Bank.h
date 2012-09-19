#ifndef _BANK_H_
#define _BANK_H_
#include <uC++.h>

_Monitor Bank {

	int *balance; 
	uCondition * sufficientFunds;

  public:
    Bank( unsigned int numStudents );
	~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif
