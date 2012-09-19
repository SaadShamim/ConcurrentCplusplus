#include <uC++.h>
#include <stdlib.h>
#include <iostream>
#include "q2tallyVotes.h"
#include "q2Printer.h"
#include "q2Voter.h"

using namespace std;

bool TallyVotes::vote(unsigned int id, bool ballot){

	//return value
	bool ret;
	
	//entry critical area
	semaphoreEntry.P();   

	//cast vote
	if(ballot == 1){
		numberOfOnes++;
		printer->print(id,Voter::Vote, ballot);
	} else {
		printer->print(id,Voter::Vote, ballot);
	}   

	//not enough voters in group
	if(blockedVoters+1 < groupSize){
		blockedVoters++;
		printer->print(id,Voter::Block, blockedVoters);

		//Allow another thread to enter and vote, whie waiting on a blocked sem
		semaphoreEntry.V();
		semaphoreWaiting.P();

		blockedVoters--;

		printer->print(id,Voter::Unblock, blockedVoters);
	}else{
		//final member of group passes through here
		printer->print(id, Voter::Complete);
	}	

	//determine the winner of vote
	if(numberOfOnes > groupSize/2 )
		ret = true;
	else
		ret = false;

	//either call group members if any are waiting, or allow groups to begin
	if(!semaphoreWaiting.empty()){
		semaphoreWaiting.V();
	} else {
		numberOfOnes = 0;
		assert(blockedVoters == 0);
		semaphoreEntry.V();
	}

	printer->print(id,Voter::Finished, ret);

	return ret;

}

TallyVotes::TallyVotes( unsigned int group, Printer &printer ):groupSize(group),printer(&printer){
	numberOfOnes =0;
	blockedVoters=0;	

#if defined( IMPLTYPE_SEM ) 
	semaphoreWaiting.P(); //decrement Waiting so its closed
#elif defined( IMPLTYPE_MC )
	useBarge=false;
#endif

}
