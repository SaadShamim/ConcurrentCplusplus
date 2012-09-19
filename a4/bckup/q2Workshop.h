#ifndef _Q2_WORKSHOP
#define _Q2_WORKSHOP
#include <uC++.h>

class Printer;

_Monitor Workshop {

   Printer *printer; 

    // private members go here
   int numberOfElves;
   int reindeerBound;
   int reindeerDeliverTime;
   
   //sleep()
   int elvesTerminated;
   int waitingConsultation;
   int waitingDelivery;
   int consecutiveDeliveries;
   uCondition santaSleep;
   
   //deliver()
   uCondition blockedReindeer;
   int deliveringReindeer;
   int reinderGroupUnblocked;
   
   //consult()
   uCondition blockedElves;
   int consultingElves;
   int groupUnblocked;
   
   //doneConsultation()
   uCondition blockedOnConsultation;
   int finishedConsulting;
   
   //doneDelivering()
   uCondition blockedOnDelivery;
   int finishedDelivering;
   
  public:
    enum Status { Consulting, Delivery, Done };
    Workshop(Printer &prt, unsigned int E, unsigned int N, unsigned int D ); // printer, elves, bound, deliveries
    Status sleep();                               // santa calls to nap; when Santa wakes status of next action
    void deliver( unsigned int id );              // reindeer call to deliver toys
    bool consult( unsigned int id );              // elves call to consult Santa,
                                                  //   true => consultation successful, false => consultation failed
    void doneConsulting( unsigned int id );       // block Santa/elves until meeting over
    void doneDelivering( unsigned int id );       // block Santa/reindeer until all toys are delivered
    void termination( unsigned int id );          // elves call to indicate termination
};

#endif

