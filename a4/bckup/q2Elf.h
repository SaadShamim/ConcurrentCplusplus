#ifndef _Q2_ELF
#define _Q2_ELF
#include <uC++.h>

class Workshop;
class Printer;

_Task Elf {
    // private members go here
    
    unsigned int id;
    unsigned int numConsultations;
    Workshop *wrk;
    Printer *printer;
    
    void main();
  public:
    enum { CONSULTING_GROUP_SIZE = 3 };           // number of elves for a consultation with Santa
    Elf( unsigned int id, Workshop &wrk, Printer &prt,unsigned int numConsultations );
};

#endif
