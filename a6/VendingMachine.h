#ifndef _VENDINGMACHINE_H_
#define _VENDINGMACHINE_H_
#include <uC++.h>

class Printer;
_Task NameServer;
class WATCard;

_Task VendingMachine {
   	//variables
	Printer *printer;
	NameServer *nameServer;
	unsigned int id;
	unsigned int sodaCost;
	unsigned int maxStockPerFlavour;
	unsigned int *stock;
	bool restocking;

	 void main();
  public:
    enum Flavours {BluesBlackCherry = 0, ClassicalCreamSoda = 1,RockRootBeer = 2,JazzLime = 3, FLAVOURS_COUNT = 4};	// flavours of soda (YOU DEFINE)
    enum Status { BUY, STOCK, FUNDS };		// purchase status: successful buy, out of stock, insufficient funds
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    Status buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
	~VendingMachine();
};

#endif
