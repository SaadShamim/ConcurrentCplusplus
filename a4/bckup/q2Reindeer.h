#ifndef _Q2_REINDEER
#define _Q2_REINDEER
#include <uC++.h>

class Workshop;
class Printer;

_Task Reindeer {
    // private members go here
    unsigned int id;
    unsigned int numDeliveries;
    Workshop *wrk;
    Printer *printer;
    
    void main();
  public:
    enum { MAX_NUM_REINDEER = 5 };                // number of reindeer in system for delivering toys
    Reindeer( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numDeliveries );
};

#endif
