//#include "q2Printer.h"
#include <uC++.h>
#include <iostream>
#include <assert.h>
#include "q2Workshop.h"
#include "q2Printer.h"
#include "q2Elf.h"
#include "q2Reindeer.h"

using namespace std;

//santa calls to nap
Workshop::Status Workshop::sleep(){
	
	//if all elves have finished and all reindeers have finished all their deliveries
	if(elvesTerminated == numberOfElves && reindeerDeliverTime == 0){
		return (Workshop::Done); // return done status
	}
	
	//number of consultation and delivering pending cant be below 0
	assert(waitingConsultation >= 0);
	assert(waitingDelivery >= 0);

	while (waitingConsultation == 0 && waitingDelivery == 0){ 
		//if all elves have finished and all reindeers have finished all their deliveries, finish 
		if(elvesTerminated == numberOfElves && reindeerDeliverTime == 0){
			return (Workshop::Done);
		}
	
		//Nap
		printer->print(0, Printer::Blocked);
		santaSleep.wait();
		printer->print(0, Printer::Unblocked);
	}
	// if both reindeer and elves need santa
	if(waitingConsultation > 0 && waitingDelivery > 0){
		//serve reindeer first, unless bound is hit
		if(consecutiveDeliveries < reindeerBound){
			consecutiveDeliveries++; 
			blockedReindeer.signal(); //wake blocked Reindeer
			return (Workshop::Delivery); //return Delivery status
		}
		//else deliver
		consecutiveDeliveries = 0;
		blockedElves.signal();
		return (Workshop::Consulting);
	}else if(waitingDelivery > 0){
		//deliver
		blockedReindeer.signal();
		return (Workshop::Delivery);
	}else if(waitingConsultation > 0){
		//consult
		blockedElves.signal();
		return (Workshop::Consulting);
	}
	
	//cases above must be met
	assert(false);
		
}

//elves waiting for santa to consult
bool Workshop::consult( unsigned int id ){
	//if all elves have finished and all reindeers have finished all their deliveries
	if(numberOfElves-elvesTerminated  < Elf::CONSULTING_GROUP_SIZE)
		return false;
	
	//keep track of waiting elves
	consultingElves++;
	assert(consultingElves != 0);

	printer->print(id, Printer::Blocked, consultingElves);
	//if waiting elves less then group, simply wait, else signal indicate waiting elves, signal santa and wait
	if(consultingElves % Elf::CONSULTING_GROUP_SIZE != 0 && consultingElves !=0){
		blockedElves.wait();
	}else{
		//signal to santa that elves waiting consultation
		waitingConsultation++;
		//if santa not sleep signal is ignored, else if sleeping wake santa
		santaSleep.signal();
		blockedElves.wait();
	}
	printer->print(id, Printer::Unblocked, consultingElves);
	groupUnblocked++; //ensures that only one group is served at a time
	consultingElves--;
	//unblock until group size limit met
	if(groupUnblocked != Elf::CONSULTING_GROUP_SIZE){
		blockedElves.signal();
	}else{
		groupUnblocked = 0;
	}
	
	//if not enough to request consultation exit the elf
	if(numberOfElves-elvesTerminated  < Elf::CONSULTING_GROUP_SIZE)
		return false; //consultation request failed
	
	//consultation success
	return true;
		
}

//reindeers waiting to deliver
void Workshop::deliver(unsigned int id){

	deliveringReindeer++;
	assert(deliveringReindeer != 0);

	
	printer->print(id, Printer::Blocked, deliveringReindeer);
	//until right number of reindeers, wait, else signal santa and request delivery
	if(deliveringReindeer % Reindeer::MAX_NUM_REINDEER != 0 && deliveringReindeer !=0){
		blockedReindeer.wait();
	}else{
		//indicator of appending delivery, used by santa
		waitingDelivery++;
		//wake santa if sleeping
		santaSleep.signal();
		blockedReindeer.wait(); //wait
	}
	printer->print(id, Printer::Unblocked, deliveringReindeer);
	//make sure only one group of reindeers served at one time
	reinderGroupUnblocked++;
	deliveringReindeer--;
	if(reinderGroupUnblocked != Reindeer::MAX_NUM_REINDEER){
		blockedReindeer.signal();
	}else{
		reinderGroupUnblocked = 0;
	}
}

//delivery complete
void Workshop::doneDelivering( unsigned int id ){
	//track how many waiting for finish
	finishedDelivering++;
	//cant have more then 5 reindeers waiting
	assert(finishedDelivering <= Reindeer::MAX_NUM_REINDEER+1);
	
	//wait if thread is santa or 1-n-1 reindeer, nth reindeer releases lock and finishes delivery 
	if(finishedDelivering < Reindeer::MAX_NUM_REINDEER+1){
		printer->print(id, Printer::Blocked, finishedDelivering);
		blockedOnDelivery.wait();
		printer->print(id, Printer::Unblocked, finishedDelivering);
	}else{
		//group of reindeeers served
		waitingDelivery--;
		//decrement amount of deliveries requested by reindeer
		reindeerDeliverTime--;
	}
	printer->print(id, Printer::DoneDelivering);
	finishedDelivering--;
	//release blocked reindeers, last reindeers signal will be lost, assert makes sure of this.
	blockedOnDelivery.signal();
}

//elves block until everyone is done consultation
void Workshop::doneConsulting( unsigned int id ){
	finishedConsulting++;
	assert(finishedConsulting <= Elf::CONSULTING_GROUP_SIZE+1);
	
	//until all elves have finished consulting, wait
	if(finishedConsulting < Elf::CONSULTING_GROUP_SIZE+1){
		printer->print(id, Printer::Blocked, finishedConsulting);
		blockedOnConsultation.wait();
		printer->print(id, Printer::Unblocked, finishedConsulting);
	}else{
		waitingConsultation--;
	}
	printer->print(id, Printer::DoneConsulting);
	finishedConsulting--;
	
	//signal waiting elves to finish consultation
	blockedOnConsultation.signal();
}

//finish elve
void Workshop::termination( unsigned int id ){
	elvesTerminated++; //track how many elves have finished
	//signal santa if not enough elf to finish group or if all elves have finished
	if(numberOfElves-elvesTerminated  < Elf::CONSULTING_GROUP_SIZE)
		blockedElves.signal();
	if(numberOfElves==elvesTerminated){
		santaSleep.signal(); 
	}
	
}


//constructor, initialize all values
Workshop::Workshop(Printer &prt, unsigned int E, unsigned int N, unsigned int D ):printer(&prt),numberOfElves(E), reindeerBound(N), reindeerDeliverTime(D){
	elvesTerminated = 0;
  	waitingConsultation = 0;
   	waitingDelivery = 0;
   	consecutiveDeliveries = 0;
   	
   	//doneConsulting
   	finishedConsulting = 0;
   	
   	//doneDelivering
   	finishedDelivering = 0;
   	
   	//consult
   	consultingElves = 0;
   	groupUnblocked = 0;
   	
   	//deliver
   	deliveringReindeer = 0;
   	reinderGroupUnblocked = 0;
   
}
