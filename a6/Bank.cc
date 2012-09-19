#include <uC++.h>
#include "Bank.h"

Bank::Bank(unsigned int numStudents):balance(new int[numStudents]) {
	for(unsigned int i=0;i<numStudents;i++){
		balance[i] = 0;
	}
	sufficientFunds = new uCondition[numStudents];
}

//deposit funds, and unblock any threads waiting to withdraw
void Bank::deposit(unsigned int id, unsigned int amount){
	balance[id] += amount;
	if (balance[id] >= 0) {
		sufficientFunds[id].signal();
	}
}

//withdraw funds, block if insufficient funds
void Bank::withdraw(unsigned int id, unsigned int amount){
	balance[id] -= amount;
	if(balance[id] < 0) {
		sufficientFunds[id].wait();
		// increasing of balance could be sufficient to wake up more
		// than one waiting withdraw request
		if (balance[id] >= 0) {
			sufficientFunds[id].signal();
		}
	}
}

Bank::~Bank() {
	delete[] sufficientFunds;
	delete[] balance;
}
