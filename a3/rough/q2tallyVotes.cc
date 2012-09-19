#include <uC++.h>
#include <stdlib.h>
#include <iostream>
#include "q2tallyVotes.h"
#include "q2Printer.h"
#include "q2Voter.h"

using namespace std;

   bool TallyVotes::vote(unsigned int id, bool ballot){

	//print start message
	//cerr << "S" << endl;
	//printer->print(id, Voter::Start);
  	bool ret;
  	
  	mlk.acquire();
    	//cerr << "acquired mlk" << endl;
    	//cerr << usrBarge << endl;
    //	cerr << "useBarge " << useBarge << endl;
    	if(useBarge)
    		bargeClk.wait(mlk);

	 if(ballot == 1){
    		numberOfOnes++;
    		//cerr << "V 1" << endl;
    		printer->print(id,Voter::Vote, ballot);
	} else {
    		//cerr << "V 0" << endl;
    		printer->print(id,Voter::Vote, ballot);
	}
	


    	if(blockedVoters+1 < groupSize){
    	    	if(!bargeClk.empty())
    			bargeClk.signal();
		else if( bargeClk.empty() && useBarge)
			useBarge = false;	
    		
    				
    		blockedVoters++;
    	//	cerr << "B " << blockedVoters << endl;
    		printer->print(id,Voter::Block, blockedVoters);
    		blockedClk.wait(mlk);
    		blockedVoters--;
			//	cerr << "U " << blockedVoters << endl;
				printer->print(id,Voter::Unblock, blockedVoters);
		if(blockedClk.empty() && useBarge)
    			useBarge = false;

	}else{
		//cerr << "C " << endl;
		printer->print(id, Voter::Complete);
	}	
	
	if(numberOfOnes > groupSize/2 )
		ret = true;
	else
		ret = false;
	
	
	
	//cerr << " prod active " << endl;
/*	
	assert(buff.size() != buffSize);
    	buff.push_back(elem);
 */ 	
    //	assert(arraySize != buffSize)
    //	bufferArray[((startIndex+arraySize)%buffSize)] = elem;
    //	arraySize++;
//    	if(entryIndex < buffSize-1)
//    		entryIndex++;
 //   	else
 //   		entryIndex=0;
    	
    	//cerr << "prod added element, size is now: " << buff.size() << endl;

	if(!blockedClk.empty()){
		useBarge = true;
	    	blockedClk.signal();
    	}else{
    		numberOfOnes = 0;
		if(!bargeClk.empty()){
		//	cerr << "blocked voters: " << blockedVoters << endl;
			assert(blockedVoters == 0);
			//******usebarge to true?
			bargeClk.signal();
		}
	}

	//cerr << " consumer signaled " << endl;

    	mlk.release();
  	
  	//cerr << "F  " << ret << endl;
  	printer->print(id,Voter::Finished, ret);
  	
	return ret;
  	
  	
  	
  	
  	
  	
  	
  	
  	
	
	/*
	int numberOfOnes =0 ;
	int numberOfBlockedVoters = 0;
	int ret;
	
	//stop bargin
	//cerr << "acquiring lock" << endl;
	mlk.acquire();
	//cerr << "acquired lock" << endl;
	
	if(numberOfBlockedVoters > 0)
    		beforeVoteClk.wait(mlk);

	//cast vote
    	if(ballot == 1){
    		numberOfOnes++;
    		cerr << "V 1" << endl;
	} else {
    		cerr << "V 0" << endl;
	}

	//cerr << "cast vote" << endl;
	
	if(numberOfBlockedVoters != groupSize-1){
		//must black and wait
		numberOfBlockedVoters++;
		cerr << "B " << numberOfBlockedVoters << endl;
		assert(beforeVoteClk.empty());
		beforeVoteClk.signal();
		atVoteClk.wait(mlk);
		numberOfBlockedVoters--;
		cerr << "U " << numberOfBlockedVoters << endl;
	}else{
		cerr << "C " << endl;
	}

	if(numberOfOnes > groupSize/2 )
		ret = 1;
	else
		ret = 0;

	
	if(!atVoteClk.empty()){
		atVoteClk.signal();	
	}else if(!beforeVoteClk.empty()){
		assert(numberOfBlockedVoters != 0);
		beforeVoteClk.signal();
	}
	
	mlk.release();

	cerr << "F  " << ret;
	return ret;
	*/	

   }
   
   TallyVotes::TallyVotes( unsigned int group, Printer &printer ):groupSize(group),printer(&printer){
     	numberOfOnes =0;
  	useBarge=false;
  	blockedVoters=0;	
   }
