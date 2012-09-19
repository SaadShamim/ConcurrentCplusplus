#ifndef _PARENT_H_
#define _PARENT_H_
#include <uC++.h>

class Printer;
class Bank;

_Task Parent {
	Printer *printer;
	Bank *bank;
	unsigned int numStudents;
	unsigned int parentalDelay;	

    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();
};


#endif
