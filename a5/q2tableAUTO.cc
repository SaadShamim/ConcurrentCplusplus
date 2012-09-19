#include <uC++.h>
#include "q2table.h"
#include "q2Printer.h"
#include <iostream>

using namespace std;

void  Table::pickup( unsigned int id ){
	//wait until both forks are avaiable
	WAITUNTIL(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false, printer->print(id, Philosopher::Waiting, id, rightFork(id)),);
	//set eating to true
	eatArray[id] = true;
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	RETURN();
}

void Table::putdown( unsigned int id ){
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	eatArray[id] = false; //set current eat status to false
	RETURN();
}

Table::Table( unsigned int noOfPhil, Printer &prt ):numberOfPhil(noOfPhil),printer(&prt),eatArray(new bool[noOfPhil]){
	for(int i =0; i<noOfPhil;i++){
		eatArray[i] = false;	//init eat array
	}
}

Table::~Table(){
	delete[] eatArray; 
}

int Table::leftFork(unsigned int id){
	if(id == 0)
		return (numberOfPhil-1);
		
	return (id-1);
}

int Table::rightFork(unsigned int id){
	if(id==numberOfPhil-1)
		return 0;
	
	return (id+1);
}
