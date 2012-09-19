#ifndef _Q2_TALLY_VOTES
#define _Q2_TALLY_VOTES
#include <uC++.h>
#include <uSemaphore.h>

#if defined( IMPLTYPE_MC )            // mutex/condition solution
// includes for this kind of vote-tallier

class Printer;

class TallyVotes {
    // private declarations for this kind of vote-tallier
    unsigned int groupSize;
    Printer *printer;  
    
    uOwnerLock mlk; 
  	uCondLock bargeClk;
  	uCondLock blockedClk;
  	
  	int numberOfOnes;
  	bool useBarge;
  	unsigned int blockedVoters;
  	
#elif defined( IMPLTYPE_SEM )        // semaphore solution
// includes for this kind of vote-tallier
class Printer;

class TallyVotes {
    // private declarations for this kind of vote-tallier

    uSemaphore semaphoreBarge;
    uSemaphore semaphoreEntry;
    uSemaphore semaphoreWaiting;
    
    int numberOfOnes;
    unsigned int blockedVoters;
    unsigned int groupSize;
    Printer *printer; 

#else
    #error unsupported voter type
#endif
    // common declarations
  public:                            // common interface
    TallyVotes( unsigned int group, Printer &printer );
    bool vote( unsigned int id, bool ballot );
};
#endif
