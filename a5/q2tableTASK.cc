#include <uC++.h>
#include "q2table.h"
#include "q2Printer.h"
#include <iostream>

using namespace std;

void  Table::pickup( unsigned int id ){
	globalId = id; //set global id, for use in main
	waitingToPickup[id].wait(); //wait
	eatArray[id] = true; //set eat status
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
}

void Table::putdown( unsigned int id ){
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	globalId = id; //set global id
}

//init values
Table::Table( unsigned int noOfPhil, Printer &prt ):numberOfPhil(noOfPhil),printer(&prt),eatArray(new bool[noOfPhil]),waitingToPickup(new uCondition[noOfPhil]),priority(new bool[noOfPhil]),amountEaten(new int[noOfPhil]){
	debug = false;
	globalId = 0;
	for(int i =0; i<noOfPhil;i++){
		eatArray[i] = false;
		priority[i] = false;
		amountEaten[i] =0;	
	}
}

//main stack
void Table::main(){
	
	for( ; ; ){
		//break condition
		_Accept(~Table){
			break;
			//change accept conditions
		}or _Accept(pickup){
			//if globalId can pickup forks
			//if(eatArray[leftFork(globalId)] == false && eatArray[rightFork(globalId)] == false){
			if( (eatArray[leftFork(globalId)] == false && eatArray[rightFork(globalId)] == false) && (priority[leftFork(globalId)] == false && priority[rightFork(globalId)] == false) ){
				if(debug)cerr << "should signal: " << globalId << endl;
				waitingToPickup[globalId].signalBlock(); //release
				if(debug)cerr << "after signal: " << globalId << endl; 
			}else{
				printer->print(globalId, Philosopher::Waiting, globalId, rightFork(globalId));
				if(debug)cerr << "not unblocking" << endl;
			}
		}or  _Accept(putdown){
			if(debug)cerr << "putting down with id: " << globalId << endl;
			assert(eatArray[leftFork(globalId)] == false && eatArray[rightFork(globalId)] == false);
			eatArray[globalId] = false;
			
			if(numberOfPhil == 3){
				//if both are waiting, pick right 
				if( ((!waitingToPickup[rightFork(globalId)].empty()) && (eatArray[rightFork(rightFork(globalId))] == false)) && ((!waitingToPickup[leftFork(globalId)].empty()) && (eatArray[leftFork(leftFork(globalId))] == false)) ){
					if(amountEaten[rightFork(globalId)] > amountEaten[leftFork(globalId)])
						waitingToPickup[leftFork(globalId)].signalBlock();
					else if (amountEaten[rightFork(globalId)] <= amountEaten[leftFork(globalId)])
						waitingToPickup[rightFork(globalId)].signalBlock();
				//signal right for blocking right
				}else if((!waitingToPickup[rightFork(globalId)].empty()) && (eatArray[rightFork(rightFork(globalId))] == false)){
					waitingToPickup[rightFork(globalId)].signalBlock();
				//signal left for blocking left	
				} else if((!waitingToPickup[leftFork(globalId)].empty()) && (eatArray[leftFork(leftFork(globalId))] == false)){
					waitingToPickup[leftFork(globalId)].signalBlock();
				}
			// >3 phil		
			}else{
				//if both neighbours blocking, signal both
				if( ((!waitingToPickup[rightFork(globalId)].empty()) && (eatArray[rightFork(rightFork(globalId))] == false)) && ((!waitingToPickup[leftFork(globalId)].empty()) && (eatArray[leftFork(leftFork(globalId))] == false)) ){
					if(debug)cerr << "signal both with id: " << globalId << endl;
					if(amountEaten[rightFork(globalId)] > amountEaten[leftFork(globalId)]){
						priority[rightFork(globalId)] = true;
						waitingToPickup[leftFork(globalId)].signalBlock();
						waitingToPickup[rightFork(globalId)].signalBlock();
					}else if (amountEaten[rightFork(globalId)] <= amountEaten[leftFork(globalId)]){
						priority[leftFork(globalId)] = true;
						waitingToPickup[rightFork(globalId)].signalBlock();
						waitingToPickup[leftFork(globalId)].signalBlock();
					
					}
				//signal right, if blocking
				}else if((!waitingToPickup[rightFork(globalId)].empty()) && (eatArray[rightFork(rightFork(globalId))] == false)){
					if(debug)cerr << "signal right with id: " << globalId << endl;
					waitingToPickup[rightFork(globalId)].signalBlock();
				//signal left, if blocking
				} else if((!waitingToPickup[leftFork(globalId)].empty()) && (eatArray[leftFork(leftFork(globalId))] == false)){
					if(debug)cerr << "signal left with id: " << globalId << endl;
					waitingToPickup[leftFork(globalId)].signalBlock();
				}
			
			}
			
			//inrement amount eaten
			amountEaten[globalId] += 1;
			//reset current priority	
			priority[globalId] = false;
			
		}
	}
	

}

Table::~Table(){
	delete[] eatArray;
	delete[] waitingToPickup;
	delete[] amountEaten;
	delete[] priority;
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
