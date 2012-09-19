#include <uC++.h>
#include "q2table.h"
#include "q2Printer.h"
#include <iostream>

using namespace std;

void  Table::pickup( unsigned int id ){
	//assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	if(debug)cerr << "pickup: " << id << endl;
	globalId = id;
	waitingToPickup[id].wait();
	eatArray[id] = true;
	if(debug)cerr << "woken" << id << endl;
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
}

void Table::putdown( unsigned int id ){
	if(debug)cerr << "putdown: " << id ;
	if(debug)cerr << " l: " << leftFork(id) << "r: " << rightFork(id) << endl;
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	globalId = id;
}

Table::Table( unsigned int noOfPhil, Printer &prt ):numberOfPhil(noOfPhil),printer(&prt),eatArray(new bool[noOfPhil]),waitingToPickup(new uCondition[noOfPhil]){
	debug = true;
	globalId = 0;
	for(int i =0; i<noOfPhil;i++){
		eatArray[i] = false;	
	}
}

//possible global id change?
void Table::main(){
	//cerr << "main" << endl;
	for( ; ; ){
		//cerr << "loop" << endl;
		_Accept(~Table){
			break;
		}or _When(true) _Accept(pickup){
			//assert(eatArray[leftFork(globalId)] == false && eatArray[rightFork(globalId)] == false);
			if(debug)cerr << "in accept" << endl;
			if(eatArray[leftFork(globalId)] == false && eatArray[rightFork(globalId)] == false){
				if(debug)cerr << "should signal: " << globalId << endl;
				//eatArray[globalId] = true;
				waitingToPickup[globalId].signalBlock();
				if(debug)cerr << "after signal: " << globalId << endl; 
				
				//wait?
			}else{
				printer->print(globalId, Philosopher::Waiting, globalId, rightFork(globalId));
				if(debug)cerr << "not unblocking" << endl;
			}
		}or _When(true) _Accept(putdown){
			if(debug)cerr << "putting down with id: " << globalId << endl;
			assert(eatArray[leftFork(globalId)] == false && eatArray[rightFork(globalId)] == false);
			eatArray[globalId] = false;
			
			if( ((!waitingToPickup[rightFork(globalId)].empty()) && (eatArray[rightFork(rightFork(globalId))] == false)) && ((!waitingToPickup[leftFork(globalId)].empty()) && (eatArray[leftFork(leftFork(globalId))] == false)) ){
				if(debug)cerr << "signal both with id: " << globalId << endl;
				waitingToPickup[rightFork(globalId)].signalBlock();
				waitingToPickup[leftFork(globalId)].signalBlock();
			}else if((!waitingToPickup[rightFork(globalId)].empty()) && (eatArray[rightFork(rightFork(globalId))] == false)){
				if(debug)cerr << "signal right with id: " << globalId << endl;
				waitingToPickup[rightFork(globalId)].signalBlock();
			} else if((!waitingToPickup[leftFork(globalId)].empty()) && (eatArray[leftFork(leftFork(globalId))] == false)){
				if(debug)cerr << "signal left with id: " << globalId << endl;
				waitingToPickup[leftFork(globalId)].signalBlock();
			}
			
		}
	}
}

Table::~Table(){
	delete[] eatArray;
	delete[] waitingToPickup;
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
