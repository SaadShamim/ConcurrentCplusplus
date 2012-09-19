#include "q2Voter.h"
#include "q2tallyVotes.h"
#include "q2Printer.h"
#include <iostream>
#include "q2PRNG.h"
using namespace std;

extern PRNG myPrng;

	void Voter::main(){

		printer->print(id, Voter::Start);
		//yield
		yield(myPrng(19));
		
		//calculate vote
		int voteResult = myPrng(1);
		
		//call for voting
		voteTallier->vote(id, voteResult);
	
	}
	
	//constructor
	Voter::Voter(unsigned int id, TallyVotes &voteTallier, Printer &printer ):id(id),voteTallier(&voteTallier),printer(&printer){}
