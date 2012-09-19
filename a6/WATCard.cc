#include <uC++.h>
#include "WATCard.h"



WATCard::WATCard() : balance(0) {

}
//deposit money
void WATCard::deposit( unsigned int amount ) {
	balance += amount;
}

//withdraw money
void WATCard::withdraw( unsigned int amount ) {
	balance -= amount;
	assert(balance >= 0);
}

//get balance
unsigned int WATCard::getBalance() {
	return balance;
}
