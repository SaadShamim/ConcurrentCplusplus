#include "q2Printer.h"
#include <iostream>
#include <uC++.h>

using namespace std;


void Printer::print( unsigned int id, States state ){

	//special case: voting finished
	if(state == Printer::Finished){
		for(unsigned int i =0;i<numberOfElves+5+1;i++){
			if(bufferArray[i] != (States)NULL){
				clearBuffer();
				break;
			}
		}
	
		for(unsigned int i=0;i<numberOfElves+5+1;i++){
			if(id != i)
				cout << "...";
			else
				cout << (char)state;
			
			if(i != numberOfElves+5)
				cout << "\t";
			bufferArray[i] = (States)NULL; //reset buffer values
			//resetNumblocked?
		}
		cout << endl;
	}else{
	
		//cerr << "\t\t\t\t\t\t\t\tid:  " << id << "state: " << (char)state << endl; 
		if(bufferArray[id] == (States)NULL){
			bufferArray[id] = state; //add to buffer array	
		}else{
			clearBuffer(); //flush if change detected and print
			bufferArray[id] = state; //store new value
		}
	
	}
	
	
}

void Printer::print( unsigned int id, States state, unsigned int numBlocked ){
	if(bufferArray[id] == (States)NULL){
		bufferArray[id] = state;	//store new value
		numBlockedBuffer[id] = numBlocked; //store number of blocked values
	}else{
		clearBuffer(); //clear buffer and print
		bufferArray[id] = state; //store new values
		numBlockedBuffer[id] = numBlocked; //number of blocked voters
	}

}

void Printer::clearBuffer(){
	for(unsigned int i=0; i<numberOfElves+5+1;i++){
		if(bufferArray[i] != (States)NULL){
			cout << (char)bufferArray[i];
			 if (bufferArray[i] == Printer::Blocked || bufferArray[i] == Printer::Unblocked) {
			 	if(numBlockedBuffer[i] != 0)
					cout << " " << numBlockedBuffer[i]; //print block or with blocking value
			}		
		}
		
		if(i != numberOfElves+5)
			cout << "\t";
		
		//reset buffer
		bufferArray[i] = (States)NULL;;
		numBlockedBuffer[i] = 0;
	}
	cout << endl;
}

Printer::Printer( const unsigned int MAX_NUM_ELVES ):numberOfElves(MAX_NUM_ELVES),bufferArray(new int[MAX_NUM_ELVES+5+1]), numBlockedBuffer(new unsigned int[MAX_NUM_ELVES+5+1]){
	//build header and initialize buffer
	cout << "Sa" << "\t";
	bufferArray[0] = (States)NULL;
	numBlockedBuffer[0]=0;
	
	for(unsigned int i=1; i<MAX_NUM_ELVES+1;i++){
		cout << "E" << i << "\t";
		bufferArray[i] = (States)NULL;
		numBlockedBuffer[i]=0;
	}
	for(unsigned int i=MAX_NUM_ELVES+1; i<MAX_NUM_ELVES+5+1;i++){
		cout << "R" << i << "\t";
		bufferArray[i] = (States)NULL;
		numBlockedBuffer[i]=0;
	}
	cout << endl;
	for(unsigned int i=0; i<MAX_NUM_ELVES+5+1;i++){
		cout << "--" << "\t";
	}
	
	cout << endl;

}

Printer::~Printer(){
	delete[] bufferArray;
   	delete[] numBlockedBuffer;
    	
	cout << "Workshop Closed" << endl;
}
