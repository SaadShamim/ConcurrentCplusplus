#include <uC++.h>
#include "q2table.h"
#include "q2Printer.h"
#include <iostream>

using namespace std;

void  Table::pickup( unsigned int id ){
	if(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false){
	
		eatArray[id] = true;
	}else{
		printer->print(id, Philosopher::Waiting, id, rightFork(id));
		
		/*for(int i=0;i<numberOfPhil;i++){
			if(waitingToEat[i] != 0){
				waitingToEat[i] += 1;
			}
		}*/
		
		waitingToEat[id] += 1;
		//assert(waitingToEat[id] == 1);
		
		waitingToPickup[id].wait();
		
		if(debug) cerr << "wake: " << id << endl;
		//waitingToEat[id] = 0;
		
		if(wakeNext != -1){
			assert(false);
			waitingToPickup[wakeNext].signal();
			wakeNext = -1;
		}
		
		assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
		eatArray[id] = true;
	}
}

void Table::putdown( unsigned int id ){
	//cerr << eatArray[leftFork(id)] << ", " <<" " <<true << "id: " << id << "bool: " << eatArray[rightFork(id)] << endl;
	assert(eatArray[leftFork(id)] == false && eatArray[rightFork(id)] == false);
	eatArray[id] = false;
	
	if(numberOfPhil == 3){
		//if you find that both neighbours are waiting on current to finish, choose the one who has eaten the least, if same 
		if( ((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)) && ((!waitingToPickup[leftFork(id)].empty()) && (eatArray[leftFork(leftFork(id))] == false)) ){
			
			if(amountEaten[rightFork(id)] > amountEaten[leftFork(id)])
				waitingToPickup[leftFork(id)].signal();
			else if (amountEaten[rightFork(id)] < amountEaten[leftFork(id)])
				waitingToPickup[rightFork(id)].signal();
			else{
				if(waitingToEat[rightFork(id)] <= waitingToEat[leftFork(id)])
					waitingToPickup[rightFork(id)].signal();
				else
					waitingToPickup[leftFork(id)].signal();
			}
		}else if((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)){
			if(debug) cerr << "waking first-2: " << rightFork(id) << endl;
			waitingToPickup[rightFork(id)].signal();
		} else if((!waitingToPickup[leftFork(id)].empty()) && (eatArray[leftFork(leftFork(id))] == false)){
			if(debug) cerr << "waking first-1: " << leftFork(id) << endl;
			waitingToPickup[leftFork(id)].signal();
		}else{
			if(debug) cerr << "waking first0: " << rightFork(id) << endl;
		}
		
		//assert(false);
	/*	if((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)){
			waitingToPickup[rightFork(id)].signal();
		} else if((!waitingToPickup[leftFork(id)].empty()) && (eatArray[leftFork(leftFork(id))] == false)){
			waitingToPickup[leftFork(id)].signal();
		}*/
	}else if(numberOfPhil == 2){
		if((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)){
			assert(rightFork(id) == leftFork(id));
			waitingToPickup[rightFork(id)].signal();
		} 
	}else {
		if( ((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)) && ((!waitingToPickup[leftFork(id)].empty()) && (eatArray[leftFork(leftFork(id))] == false)) ){
			
			/*if(amountEaten[rightFork(id)] < amountEaten[leftFork(id)]){
				
				//waitingToPickup[leftFork(id)].signal();
				wakeNext = leftFork(id);
				waitingToPickup[rightFork(id)].signal();
			}else{
				
				//waitingToPickup[rightFork(id)].signal();
				wakeNext = rightFork(id);
				waitingToPickup[leftFork(id)].signal();
			}*/
			
			if(amountEaten[rightFork(id)] > amountEaten[leftFork(id)]){
				
				//waitingToPickup[leftFork(id)].signal();
				//wakeNext = rightFork(id);
				waitingToPickup[rightFork(id)].signal();
				waitingToPickup[leftFork(id)].signal();
			}else if (amountEaten[rightFork(id)] < amountEaten[leftFork(id)]){
				
				waitingToPickup[leftFork(id)].signal();
				waitingToPickup[rightFork(id)].signal();
				
				//wakeNext = leftFork(id);
				
			}else{
				if(waitingToEat[rightFork(id)] < waitingToEat[leftFork(id)]){
					//wakeNext = leftFork(id);
					waitingToPickup[leftFork(id)].signal();
					waitingToPickup[rightFork(id)].signal();
				}else if(waitingToEat[rightFork(id)] > waitingToEat[leftFork(id)]){
					//wakeNext = rightFork(id);
					waitingToPickup[rightFork(id)].signal();
					waitingToPickup[leftFork(id)].signal();
				}else{
					//wakeNext = rightFork(id);
					waitingToPickup[rightFork(id)].signal();
					waitingToPickup[leftFork(id)].signal();
				}
			
			}
			
			/*if(waitingToEat[rightFork(id)] > waitingToEat[leftFork(id)]){
				if(debug) cerr << "waking first1: " << rightFork(id) << endl;
				
				waitingToPickup[leftFork(id)].signal();
				waitingToPickup[rightFork(id)].signal();
				//waitingToPickup[leftFork(id)].signal();
				
			}else{
				if(debug) cerr << "waking first2: " << leftFork(id) << endl;
				assert(waitingToEat[rightFork(id)] < waitingToEat[leftFork(id)]);
				assert(waitingToEat[rightFork(id)] != waitingToEat[leftFork(id)]);
				
				waitingToPickup[rightFork(id)].signal();
				waitingToPickup[leftFork(id)].signal();
				
			}*/
		}else{
			if(debug) cerr << "waking first3: " << leftFork(id) << endl;
			if((!waitingToPickup[rightFork(id)].empty()) && (eatArray[rightFork(rightFork(id))] == false)){
				waitingToPickup[rightFork(id)].signal();
			} else if((!waitingToPickup[leftFork(id)].empty()) && (eatArray[leftFork(leftFork(id))] == false)){
				waitingToPickup[leftFork(id)].signal();
			}
		}
	}
	amountEaten[id] += 1;	
	//need starvation condition;
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

Table::Table( unsigned int noOfPhil, Printer &prt ):numberOfPhil(noOfPhil),printer(&prt), eatArray(new bool[noOfPhil]),waitingToPickup(new uCondition[noOfPhil]),waitingToEat(new int[noOfPhil]),amountEaten(new int[noOfPhil]){
	for(int i =0; i<noOfPhil;i++){
		eatArray[i] = false;	
		waitingToEat[i] = 0;
		amountEaten[i] =0;
	}
	debug = false;
	wakeNext = -1;
}

Table::~Table(){
	delete[] amountEaten;
	delete[] eatArray;
   	delete[] waitingToPickup;
   	delete[] waitingToEat;
}
