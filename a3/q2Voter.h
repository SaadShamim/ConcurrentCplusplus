#ifndef _Q2_VOTERS
#define _Q2_VOTERS
#include <uC++.h>
//#include "q2tallyVotes.h"
//#include "q2Printer.h"
class Printer;
class TallyVotes;
_Task Voter {
   unsigned int id;
   TallyVotes *voteTallier;
   Printer *printer;

	void main();
  public:
    enum States { Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U', Complete = 'C', Finished = 'F' };
    Voter( unsigned int id, TallyVotes &voteTallier, Printer &printer );
};
#endif
