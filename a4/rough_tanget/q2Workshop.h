#ifndef _Q2_WORKSHOP
#define _Q2_WORKSHOP
#include <uC++.h>

class Printer;

_Monitor Workshop {

   Printer *printer; 

    // private members go here
   int numberOfElves;
   int reindeerBound; //N
   int reindeerDeliverTime; //D
   
   //sleep()
   int elvesTerminated; // how many elves have finished
   int waitingConsultation; //elves waiting for Consultation
   int waitingDelivery; //elves waiting for Delivery
   int consecutiveDeliveries; //keeps track of consecutive delivery for reindeer; prevents starvation
   uCondition santaSleep; //lock where santa sleeps
   
   //deliver()
   uCondition blockedReindeer; //waiting reindeer
   int deliveringReindeer; //number of waiting reindeer
   int reinderGroupUnblocked; //number released
   
   //consult()
   uCondition blockedElves; //waiting elves
   int consultingElves; //number of waiting elves
   int groupUnblocked; //track groups of reindeer
   
   //doneConsultation()
   uCondition blockedOnConsultation; //waiting for all elves to finish 
   int finishedConsulting; //waiting on consultation
   
   //doneDelivering()
   uCondition blockedOnDelivery; //waiting for all reindeers to finish
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

