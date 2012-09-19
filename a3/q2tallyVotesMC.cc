#include <uC++.h>
#include <stdlib.h>
#include <iostream>
#include "q2tallyVotes.h"
#include "q2Printer.h"
#include "q2Voter.h"

using namespace std;

bool TallyVotes::vote(unsigned int id, bool ballot){

	bool ret;

	mlk.acquire();

	//prevent race condition
	if(useBarge)
		bargeClk.wait(mlk);
		
	//cast vote
	if(ballot == 1){
		numberOfOnes++;
		printer->print(id,Voter::Vote, ballot);
	} else {
		printer->print(id,Voter::Vote, ballot);
	}


	//wait for group members, if not enough voters
	if(blockedVoters+1 < groupSize){
		//envoke barge if possible
		if(!bargeClk.empty())
			bargeClk.signal();
		else if( bargeClk.empty() && useBarge)
			useBarge = false;	

		//cast vote
		blockedVoters++;
		
		printer->print(id,Voter::Block, blockedVoters);
		blockedClk.wait(mlk);	//wait from group to finish voting
		blockedVoters--;

		printer->print(id,Voter::Unblock, blockedVoters);
		if(blockedClk.empty() && useBarge)
			useBarge = false;

	}else{

		printer->print(id, Voter::Complete);
	}	

	//decide on winner of vote
	if(numberOfOnes > groupSize/2 )
		ret = true;
	else
		ret = false;
	
	//call waiting group members
	if(!blockedClk.empty()){
		useBarge = true;
		blockedClk.signal();
	}else{
		numberOfOnes = 0;
		if(!bargeClk.empty()){
			//call threads in barge
			assert(blockedVoters == 0);
			bargeClk.signal();
		}
	}


	mlk.release();


	printer->print(id,Voter::Finished, ret);

	return ret;


}

TallyVotes::TallyVotes( unsigned int group, Printer &printer ):groupSize(group),printer(&printer){
	numberOfOnes =0;
	useBarge=false;
	blockedVoters=0;	
}
