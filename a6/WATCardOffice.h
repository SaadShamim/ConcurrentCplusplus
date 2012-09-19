#ifndef _WATCARD_OFFICE_H_
#define _WATCARD_OFFICE_H_

#include <uC++.h>
#include <uSemaphore.h>
#include "WATCard.h"
#include <queue>

class Bank;
class Printer;
class Job;

_Task WATCardOffice {
		struct Job { // marshalled arguments and return future
			FWATCard result; // return future
			WATCard * card;
			unsigned int id;
			unsigned int amount;		// withdrawal amount
			Job( FWATCard result, WATCard * card, unsigned int id, unsigned int amount );
		};
		_Task Courier {
			WATCardOffice * office;
			Bank * bank;
			unsigned int id;
			void main();
			public:
				Courier( WATCardOffice *office, Bank &bank, unsigned int id );
		}; // Courier
		Printer * prt;
		Bank * bank;
		unsigned int numCouriers;
		uCondition jobAvailable;
		std::queue<Job*> jobs;
		Courier ** couriers;
		bool officeShuttingDown;
		void main();
	public:
		_Event Lost {};
		WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
		FWATCard create( unsigned int sid, unsigned int amount);
		FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card);
		Job *requestWork();
		~WATCardOffice();
};


#endif

