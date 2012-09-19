#ifndef _Q2_SANTA
#define _Q2_SANTA
#include <uC++.h>

class Workshop;
class Printer;

_Task Santa {
    // private members go here
    Workshop *wrk; //ptr to workshop (monitor)
     Printer *printer; //ptr to printer
     
    void main();
  public:
    Santa( Workshop &wrk, Printer &prt);
};

#endif
