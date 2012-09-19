#include <uC++.h>
#include <uFuture.h>
#include "Bank.h"
#include "Printer.h"
#include "WATCardOffice.h"
#include <iostream>
#include "MPRNG.h"

using namespace std;

extern MPRNG mprng;

void WATCardOffice::main() {
	prt->print(Printer::WATCardOffice, 'S');
	while(true) {
		_Accept(create) {
		} or _Accept(transfer) {
		} or _Accept(requestWork) {
//			prt->print(Printer::WATCardOffice, 'W');
		} or _Accept(~WATCardOffice) {
			officeShuttingDown = true;
			break;
		}
	}
	prt->print(Printer::WATCardOffice, 'F');
}

//WATCardOffice constructor
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : 
	prt(&prt), bank(&bank), numCouriers(numCouriers), officeShuttingDown(false) {
	couriers = new Courier * [numCouriers];
	for (unsigned int i = 0; i < numCouriers; i++) {
		couriers[i] = new Courier(this,bank, i);
	}
}

WATCardOffice::~WATCardOffice() {
	if(!jobAvailable.empty()) {
		jobAvailable.signalBlock();
	}
	for (unsigned int i = 0; i < numCouriers; i++) {
		jobAvailable.signal();
		delete couriers[i];	
	}
	delete[] couriers;
	while(!jobs.empty()) {
		Job * job = jobs.front();
		delete job;
		jobs.pop();
	}
}

// Job's constructor
WATCardOffice::Job::Job( FWATCard result, WATCard *card, unsigned int id, unsigned int amount) : 
		result(result), card(card), id(id), amount(amount) {

}

FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
	FWATCard theFuture;
	jobs.push((Job *) new Job(theFuture, new WATCard(), sid, amount));
	jobAvailable.signal();
	prt->print(Printer::WATCardOffice, 'C', (int) sid, (int) amount);
	return theFuture;
}

FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard* card ) {
	FWATCard theFuture;
	jobs.push(new Job(theFuture, card, sid, amount));
	jobAvailable.signal();
	prt->print(Printer::WATCardOffice, 'T', (int) sid, (int) amount);
	return theFuture;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
	if(jobs.empty() && !officeShuttingDown) {
		jobAvailable.wait();
	}
	if (officeShuttingDown) {
		jobAvailable.signal();
		return NULL;
	}
	Job * job = jobs.front();
	jobs.pop();
	return job;
}

WATCardOffice::Courier::Courier( WATCardOffice *office, Bank &bank, unsigned int id) : office(office), bank(&bank), id(id) {

}

// courier's main
void WATCardOffice::Courier::main() {
	office->prt->print(Printer::Courier, id, 'S');
	Job * job;
	unsigned int numTimesLost = 0;
	while(true) {
		job = office->requestWork();
		if(job == NULL) {
			//cerr << "null office" << endl;
			break;
		}
		office->prt->print(Printer::Courier, id, 't', (int) job->id, (int) job->amount);
		// head to the bank and withdraw the amount in the job
		bank->withdraw( job->id, job->amount);
		office->prt->print(Printer::Courier, id, 'T', (int) job->id, (int) job->amount);
		WATCard * theCard = job->card;
		theCard->deposit(job->amount);
		// simulate possibly losing the card
		if(mprng(5) == 0) {
			numTimesLost++;
			//assert(numTimesLost < 2);
			// the number 0 has a 1/6 probability of occuring 
			// lost the card
			delete theCard;
			//job->result.reset();
			job->result.exception(new Lost);
			delete job;
			continue;
			//return;
		}
		// deliver the watcard to the future
		job->result.delivery(theCard);
		delete job;
		office->prt->print(Printer::WATCardOffice, 'W');
	}
}
