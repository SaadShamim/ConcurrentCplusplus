#ifndef _Q2_PRINTER
#define _Q2_PRINTER
#include <uC++.h>

//class Voter;
#include "q2Voter.h"

_Monitor Printer{     // chose one of the two kinds of type constructor
	int numberOfVoters;
	 
	 int *bufferArray;
	 bool *voteBufferArray;
	 unsigned int *numBlockedBufferArray;
	 
	 void clearBuffer();
	 void addSpacing(int id);
	
  public:
    Printer( unsigned int voters );
    void print( unsigned int id, Voter::States state );
    void print( unsigned int id, Voter::States state, bool vote );
    void print( unsigned int id, Voter::States state, unsigned int numBlocked ); 
    ~Printer();
};
#endif
