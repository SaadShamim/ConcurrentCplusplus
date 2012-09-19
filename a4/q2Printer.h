#ifndef _Q2_PRINTER
#define _Q2_PRINTER
#include <uC++.h>

_Monitor Printer {                  // choose monitor or cormonitor
 	
 	unsigned int *numBlockedBuffer;
 	unsigned int numberOfElves;
 	
 	void clearBuffer();
 	
  public:
    enum States { Starting = 'S', Blocked = 'B', Unblocked = 'U', Finished = 'F', // general
          Napping = 'N', Awake = 'A',             // Santa
          Working = 'W', NeedHelp = 'H',          // elf
          OnVacation = 'V', CheckingIn = 'I',     // reindeer
          DeliveringToys = 'D', DoneDelivering = 'd', // Santa, reindeer
          Consulting = 'C', DoneConsulting = 'c', // Santa, elves
          ConsultingFailed = 'X',                 // elf
    };
    Printer( const unsigned int MAX_NUM_ELVES );
    void print( unsigned int id, States state );
    void print( unsigned int id, States state, unsigned int numBlocked );
    ~Printer();
    
    private:
    States *bufferArray;
};

#endif
