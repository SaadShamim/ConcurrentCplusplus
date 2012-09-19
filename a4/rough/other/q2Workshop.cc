//#include "q2Printer.h"
#include <uC++.h>
#include <iostream>
#include <assert.h>
#include "q2Workshop.h"
#include "q2Printer.h"

using namespace std;

Workshop::Status Workshop::sleep(){
	//if done
	//alternative: decrement rendeerDeliverTime?
	if(elvesTerminated == numberOfElves && reindeerDeliverTime == 0){
		return (Workshop::Done);
	}
	
	//handle dead lock case (blocked elves)
	//do I need a while?
	
	//cerr << waitingConsultation << endl;
	assert(waitingConsultation >= 0);
	assert(waitingDelivery >= 0);
	//cerr << "sleeping" << endl;
	while (waitingConsultation == 0 && waitingDelivery == 0){ 
	//cerr << "slept" << endl;
		if(elvesTerminated == numberOfElves && reindeerDeliverTime == 0){
			return (Workshop::Done);
		}
	
	//	cerr << "wc: " << waitingConsultation << endl;
	//	cerr << "minus: " <<  numberOfElves-elvesTerminated << endl;
	//	cerr << "consultingElves: " << consultingElves << endl;
	//	cerr << "waiting Consultation: " << waitingConsultation << endl;
	
		//cerr << "Santa N" << endl;
		//cerr << waitingConsultation << endl;
		//cerr << waitingDelivery << endl;
		//cerr << elvesTerminated << " " << numberOfElves << endl;
		//cerr << "consultingElves: " << consultingElves << endl;
		printer->print(0, Printer::Blocked);
		//cerr << "sleeping" << endl;
		santaSleep.wait();
		printer->print(0, Printer::Unblocked);
	}
	//printer->print(0, Printer::Awake);
	if(waitingConsultation > 0 && waitingDelivery > 0){
		if(consecutiveDeliveries < reindeerBound){
			consecutiveDeliveries++;
			//waitingDelivery--;
			cerr << "Santa D" << endl;
			return (Workshop::Delivery);
		}
		//else
		//waitingConsultation--;
		consecutiveDeliveries = 0;
		cerr << "SantaC" << endl;
		return (Workshop::Consulting);
	}else if(waitingDelivery > 0){
	//	waitingDelivery--;
		//cerr << "Santa D" << endl;
		//printer->print(0, Printer::DeliveringToys);
		blockedReindeer.signal();
		return (Workshop::Delivery);
	}else if(waitingConsultation > 0){
		//waitingConsultation--;
		//cerr << "Santa C" << endl;
		//printer->print(0, Printer::Consulting);
		blockedElves.signal();
		return (Workshop::Consulting);
	}
	
	cerr << waitingConsultation << " " << waitingDelivery << endl;
	assert(false);
	//cerr << "ASSERT" << endl;
		
}

bool Workshop::consult( unsigned int id ){
	//check against output
	if(numberOfElves-elvesTerminated  < 3)
		return false;

	consultingElves++;
	//cerr << consultingElves << endl;
	assert(consultingElves != 0);
	//use groupSize varaible;
	printer->print(id, Printer::Blocked, consultingElves);
	//if(consultingElves % 3 != 0){
	if(consultingElves % 3 != 0 && consultingElves !=0){
		blockedElves.wait();
	}else{
		//cerr << "waiting consultation: " << id << " consultinveElves: " << consultingElves << endl;
		waitingConsultation++;
		//if santa thread not on sleep;
		santaSleep.signal();
		blockedElves.wait();
	}
	printer->print(id, Printer::Unblocked, consultingElves);
	groupUnblocked++;
	consultingElves--;
	if(groupUnblocked != 3){
		blockedElves.signal();
	}else{
		groupUnblocked = 0;
	}
	//consultingElves--;
	//do I really need this?
	if(numberOfElves-elvesTerminated  < 3)
		return false;
	
	return true;
		
}

void Workshop::deliver(unsigned int id){

	deliveringReindeer++;
	//cerr << consultingElves << endl;
	assert(deliveringReindeer != 0);
	//use groupSize varaible;
	printer->print(id, Printer::Blocked, deliveringReindeer);
	//if(consultingElves % 3 != 0){
	//deliveryReinder not need, assert up top
	if(deliveringReindeer % 5 != 0 && deliveringReindeer !=0){
		blockedReindeer.wait();
	}else{
		//cerr << "waiting consultation: " << id << " consultinveElves: " << consultingElves << endl;
		waitingDelivery++;
		//if santa thread not on sleep;
		santaSleep.signal();
		blockedReindeer.wait();
	}
	printer->print(id, Printer::Unblocked, deliveringReindeer);
	reinderGroupUnblocked++;
	deliveringReindeer--;
	if(reinderGroupUnblocked != 5){
		blockedReindeer.signal();
	}else{
		reinderGroupUnblocked = 0;
	}
}

void Workshop::doneDelivering( unsigned int id ){
	//blocking print message?
	//replace with enum
	finishedDelivering++;
	assert(finishedDelivering <= 5+1);
	if(finishedDelivering < 5+1){
		//cerr << "B: " << finishedConsulting << endl;
		//if(id == 0)
			//blockedReindeer.signal();
		printer->print(id, Printer::Blocked, finishedDelivering);
		blockedOnDelivery.wait();
		printer->print(id, Printer::Unblocked, finishedDelivering);
	}else{
		//cerr << id << "c" << endl;
		waitingDelivery--;
		reindeerDeliverTime--;
	}
	printer->print(id, Printer::DoneDelivering);
	//cerr << id << "c" << endl;
	finishedDelivering--;
	//last member of group, no one waiting, signal anyway?
	blockedOnDelivery.signal();
	
//	if(reindeerDeliverTime == 0 && finishedDelivering == 0)
//		santaSleep.signal();
		
	//what if more then group size in blockedElves? mod? rather then 3+1 or can only 3 come in at a time
}

void Workshop::doneConsulting( unsigned int id ){
	//blocking print message?
	//replace with enum
	finishedConsulting++;
	assert(finishedConsulting <= 3+1);
	if(finishedConsulting < 3+1){
		//cerr << "B: " << finishedConsulting << endl;
		//if(id == 0)
			//blockedElves.signal();
		printer->print(id, Printer::Blocked, finishedConsulting);
		blockedOnConsultation.wait();
		printer->print(id, Printer::Unblocked, finishedConsulting);
	}else{
		//cerr << id << "c" << endl;
		waitingConsultation--;
	}
	printer->print(id, Printer::DoneConsulting);
	//cerr << id << "c" << endl;
	finishedConsulting--;
	//last member of group, no one waiting, signal anyway?
	blockedOnConsultation.signal();
	
	//what if more then group size in blockedElves? mod? rather then 3+1 or can only 3 come in at a time
}

void Workshop::termination( unsigned int id ){
	//cerr << "wc: " << waitingConsultation << endl;
	//cerr << "minus: " <<  numberOfElves-elvesTerminated << endl;
	//cerr << "consultingElves: " << consultingElves << endl;
	elvesTerminated++;
	if(numberOfElves-elvesTerminated  < 3)
		blockedElves.signal();
	if(numberOfElves==elvesTerminated){
	//cerr<< "waiting: " << waitingConsultation << endl;
		santaSleep.signal(); //incase santa sleeping?
	}
	//cerr << "elves terminated " << numberOfElves-elvesTerminated << endl;
}

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
