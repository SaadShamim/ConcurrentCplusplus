#include <uC++.h>
#include "q2table.h"
#include "q2Printer.h"
#include <iostream>

using namespace std;

void  Table::pickup( unsigned int id ){
	WAITUNTIL(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false, printer->print(id, Philosopher::Waiting, id, rightFork(id)),);
	eatArray[id] = true;
	//cerr << "current id: " << id << " l:" << eatArray[leftFork(id)]<< ",r: " << eatArray[rightFork(id)] << endl;
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	RETURN();
}

void Table::putdown( unsigned int id ){
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	eatArray[id] = false;
	//cerr << "set id: " << id << " to false" << endl;
	RETURN();
}

Table::Table( unsigned int noOfPhil, Printer &prt ):numberOfPhil(noOfPhil),printer(&prt),eatArray(new bool[noOfPhil]){
	for(int i =0; i<noOfPhil;i++){
		eatArray[i] = false;	
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
