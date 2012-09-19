#include <uC++.h>
#include "q2table.h"
#include "q2Printer.h"
#include <iostream>

using namespace std;

void  Table::pickup( unsigned int id ){
	//check for empty fork, and no neighbours are given priority
	if( (eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false) && (priority[leftFork(id)] == false && priority[rightFork(id)] == false)){
		eatArray[id] = true;
	}else{
		printer->print(id, Philosopher::Waiting, id, rightFork(id));
		
		waitingToPickup[id].wait();
		
		if(debug) cerr << "wake: " << id << endl;
		assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
		eatArray[id] = true;
	}
}

void Table::putdown( unsigned int id ){
	//cerr << eatArray[leftFork(id)] << ", " <<" " <<true << "id: " << id << "bool: " << eatArray[rightFork(id)] << endl;
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	eatArray[id] = false;
	
	if(numberOfPhil == 3){
		//if you find that both neighbours are waiting on current to finish, choose the one who has eaten the least
		if( ((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)) && ((!waitingToPickup[leftFork(id)].empty()) && (eatArray[leftFork(leftFork(id))] == false)) ){
			if(amountEaten[rightFork(id)] > amountEaten[leftFork(id)])
				waitingToPickup[leftFork(id)].signal();
			else if (amountEaten[rightFork(id)] <= amountEaten[leftFork(id)])
				waitingToPickup[rightFork(id)].signal();
		//if right is blocked		
		}else if((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)){
			
			waitingToPickup[rightFork(id)].signal();
		//if left is blocked
		} else if((!waitingToPickup[leftFork(id)].empty()) && (eatArray[leftFork(leftFork(id))] == false)){
		
			waitingToPickup[leftFork(id)].signal();
		}
	}else if(numberOfPhil == 2){
		//always choose the next one
		if((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)){
			assert(rightFork(id) == leftFork(id));
			waitingToPickup[rightFork(id)].signal();
		} 
	}else { // case > 3
		//if both are blocked, give pref to one who has eaten the least
		if( ((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)) && ((!waitingToPickup[leftFork(id)].empty()) && (eatArray[leftFork(leftFork(id))] == false)) ){
		
			//unlock both, but prefer the less eaten one first
			if(amountEaten[rightFork(id)] > amountEaten[leftFork(id)]){
				//set priority ticket for unpreferred node
				priority[rightFork(id)] = true;
				waitingToPickup[rightFork(id)].signal();
				waitingToPickup[leftFork(id)].signal();
			}else if (amountEaten[rightFork(id)] <= amountEaten[leftFork(id)]){
				//set priority ticket for unpreferred node
				priority[leftFork(id)] = true;
				waitingToPickup[leftFork(id)].signal();
				waitingToPickup[rightFork(id)].signal();
			}
		}else{
			//release either left or right depending on condition
			if((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)){
				waitingToPickup[rightFork(id)].signal();
			} else if((!waitingToPickup[leftFork(id)].empty()) && (eatArray[leftFork(leftFork(id))] == false)){
				waitingToPickup[leftFork(id)].signal();
			}
		}
	}
	//inrement amount eaten
	amountEaten[id] += 1;
	//reset current priority	
	priority[id] = false;
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

//constructor
Table::Table( unsigned int noOfPhil, Printer &prt ):numberOfPhil(noOfPhil),printer(&prt), eatArray(new bool[noOfPhil]),waitingToPickup(new uCondition[noOfPhil]),amountEaten(new int[noOfPhil]), priority(new bool[noOfPhil]){
	for(int i =0; i<noOfPhil;i++){
		eatArray[i] = false;	
		priority[i] = false;
		amountEaten[i] =0;
	}
	debug = false;
}

//free memory
Table::~Table(){
	delete[] amountEaten;
	delete[] eatArray;
   	delete[] waitingToPickup;
   	delete[] priority;
}
