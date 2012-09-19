#ifndef _Q2_REINDEER
#define _Q2_REINDEER
#include <uC++.h>

class Workshop;
class Printer;

_Task Reindeer {
    // private members go here
    unsigned int id; //id
    unsigned int numDeliveries; //number of deliveries
    Workshop *wrk; //ptr to workshop (monitor)
    Printer *printer; //ptr to printer
    
    void main();
  public:
    enum { MAX_NUM_REINDEER = 5 };                // number of reindeer in system for delivering toys
    Reindeer( unsigned int id, Workshop &wrk, Printer &prt, unsigned int numDeliveries );
};

#endif
