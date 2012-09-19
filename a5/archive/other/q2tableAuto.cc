#include <uC++.h>
#include "q2table.h"
#include "q2Printer.h"
#include <iostream>

using namespace std;

void  Table::pickup( unsigned int id ){
	WAITUNTIL(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false, printer->print(id, Philosopher::Waiting, id, rightFork(id)),eatArray[id] = true);
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	RETURN();
}

void Table::putdown( unsigned int id ){
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	eatArray[id] = false;
	RETURN();
}

Table::Table( unsigned int noOfPhil, Printer &prt ):numberOfPhil(noOfPhil),printer(&prt){}

Table::~Table(){}
