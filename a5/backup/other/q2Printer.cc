#include "q2Printer.h"
//#include "q2Philosopher.h"
#include <iostream>
#include <uC++.h>

using namespace std;

void Printer::print( unsigned int id, Philosopher::States state ){
	//special case: voting finished
	if(state == Philosopher::Finished){
		for(unsigned int i =0;i<numberOfPhil;i++){
			if(bufferArray[i] != (Philosopher::States)NULL){
				clearBuffer();
				break;
			}
		}
	
		for(unsigned int i=0;i<numberOfPhil;i++){
			if(id != i)
				cout << "...";
			else
				cout << (char)state;
			
			if(i != numberOfPhil-1)
				cout << "\t";
				
			bufferArray[i] = (Philosopher::States)NULL; //reset buffer values
			leftValue[i] = (Philosopher::States)NULL;
			rightValue[i] = (Philosopher::States)NULL;
		}
		cout << endl;
	}else{
	
		//print if new value, else add to buffer
		if(bufferArray[id] == (Philosopher::States)NULL){
			bufferArray[id] = state; //add to buffer array	
		}else{
			clearBuffer(); //flush if change detected and print
			bufferArray[id] = state; //store new value
		}
	
	}

}

void Printer::print( unsigned int id, Philosopher::States state, unsigned int bite, unsigned int noodles ){
	if(bufferArray[id] == (Philosopher::States)NULL){
		bufferArray[id] = state;	//store new value
		leftValue[id] = bite; 
		rightValue[id] = noodles;
	}else{
		clearBuffer(); //clear buffer and print
		bufferArray[id] = state; //store new values
		leftValue[id] = bite; 
		rightValue[id] = noodles;
	}
}


void Printer::print_tabs(int tabs)
{
	for (int i = 0; i < tabs; i++) {
		cout << "\t";
	}
}

void Printer::clearBuffer(){
	int tabs = 0;
	for(unsigned int i=0; i<numberOfPhil;i++){
		if(bufferArray[i] != (Philosopher::States)NULL){
			print_tabs(tabs);
			cout << (char)bufferArray[i];
			 if (bufferArray[i] == Philosopher::Waiting || bufferArray[i] == Philosopher::Eating) {
					cout  << leftValue[i] << "," << rightValue[i]; //print block or with blocking value
			}
			tabs=1;
		} else {
			tabs++;
		}
		
		//reset buffer
		bufferArray[i] = (Philosopher::States)NULL;
		leftValue[i] = (Philosopher::States)NULL;
		rightValue[i] = (Philosopher::States)NULL;
		
	}
	cout << endl;
}

Printer::Printer( unsigned int NoOfPhil ):numberOfPhil(NoOfPhil),bufferArray(new int[NoOfPhil]), leftValue(new unsigned int[NoOfPhil]), rightValue(new unsigned int[NoOfPhil]){
	//build header and initialize buffer
	int tabs = 0;
	for(int i=0; i<NoOfPhil;i++){
		print_tabs(tabs);
		if (i == NoOfPhil - 1) cout << "Phil" << i;
		else cout << "Phil" << i << "\t";
		bufferArray[i] = (Philosopher::States)NULL;
		leftValue[i] = (Philosopher::States)NULL;
		rightValue[i] = (Philosopher::States)NULL;
	}
	cout << endl;
	for(int i=0; i<NoOfPhil;i++){
		if (i == NoOfPhil - 1) cout << "******";
		else cout << "******" << "\t";
	}
	
	cout << endl;
}

//remove printer and print closing argument
Printer::~Printer(){
	delete[] bufferArray;
   	delete[] leftValue;
   	delete[] rightValue;
    	
    	cout << "***********************" << endl;
	cout << "Philosophers terminated" << endl;
}
