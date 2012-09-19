#include "q2Printer.h"
#include <iostream>

using namespace std;

//accounts for Start and Complete
void Printer::print( unsigned int id, Voter::States state ){
	if(bufferArray[id] == (Voter::States)NULL){
		bufferArray[id] = state; //add to buffer array	
	}else{
		clearBuffer(); //flush if change detected and print
		bufferArray[id] = state; //store new value
	}
}	

//for voting and finishing
void Printer::print( unsigned int id, Voter::States state, bool vote ){
	if(bufferArray[id] == (Voter::States)NULL){
		bufferArray[id] = state;	 //store enum
		voteBufferArray[id] = vote;  //store vote value
	}else{
		clearBuffer(); //clear and print
		bufferArray[id] = state; //store new state
		voteBufferArray[id] = vote; //store vote value
	}
	
	//special case: voting finished
	if(state == Voter::Finished){
		for(int i=0;i<numberOfVoters;i++){
			if(id != i)
				cout << "...";
			else
				cout << (char)state << " " << vote;
			
			if(i != numberOfVoters-1)
				cout << "\t";
			bufferArray[i] = (Voter::States)NULL; //reset buffer values
		}
		cout << endl;
	}
}

//For block and unblock
void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ){
	if(bufferArray[id] == (Voter::States)NULL){
		bufferArray[id] = state;	//store new value
		numBlockedBufferArray[id] = numBlocked; //store number of blocked values
	}else{
		clearBuffer(); //clear buffer and print
		bufferArray[id] = state; //store new values
		numBlockedBufferArray[id] = numBlocked; //number of blocked voters
	}
}

Printer::Printer( unsigned int voters ):numberOfVoters(voters),bufferArray(new int[voters]), voteBufferArray(new bool[voters]), numBlockedBufferArray(new unsigned int[voters]){
	//build header and initialize buffer
	for(int i=0; i<voters;i++){
		cout << "Voter" << i << "\t";
		bufferArray[i] = (Voter::States)NULL;
		voteBufferArray[i] = false;
		numBlockedBufferArray[i]=0;
	}
	cout << endl;
	for(int i=0; i<voters;i++){
		cout << "=======" << "\t";
	}
	
	cout << endl;
}

//clear buffer and print its contents
void Printer::clearBuffer(){
	for(int i=0; i<numberOfVoters;i++){
		if(bufferArray[i] != (Voter::States)NULL){
			cout << (char)bufferArray[i];
			if (bufferArray[i] == Voter::Vote){
				cout << " " << voteBufferArray[i]; //print vote with vote value
			}else if (bufferArray[i] == Voter::Block || bufferArray[i] == Voter::Unblock) {
				cout << " " << numBlockedBufferArray[i]; //print block or with blocking value
			}		
		}
		
		if(i != numberOfVoters-1)
			cout << "\t";
		
		//reset buffer
		bufferArray[i] = (Voter::States)NULL;;
		voteBufferArray[i] = false;
		numBlockedBufferArray[i] = 0;
	}
	cout << endl;
}

//clear memory and print footer
Printer::~Printer(){
	delete[] bufferArray;
    delete[] voteBufferArray;
    delete[] numBlockedBufferArray;
    	
	cout << "=================" << endl;
    cout << "All tours started" << endl;
}

