#ifndef _Q2_PHILOSOPHER
#define _Q2_PHILOSOPHER
#include <uC++.h>
#include "q2table.h"

//class Table;
class Printer;

_Task Philosopher {
	
  Printer *printer; //ptr to printer
  Table *table;
  unsigned int numberOfNoodles;
  unsigned int id;

 void main();
  public:
    enum States { Thinking = 'T', Hungry = 'H', Eating ='E', Waiting = 'W', Finished = 'F' };
    Philosopher( unsigned int id, unsigned int noodles, Table &table, Printer &prt );
};

#endif
