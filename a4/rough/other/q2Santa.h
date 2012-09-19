#ifndef _Q2_SANTA
#define _Q2_SANTA
#include <uC++.h>

class Workshop;
class Printer;

_Task Santa {
    // private members go here
    Workshop *wrk;
     Printer *printer;
     
    void main();
  public:
    Santa( Workshop &wrk, Printer &prt);
};

#endif
