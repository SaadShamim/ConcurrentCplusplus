#include <uC++.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;					// direct access to std
#include <cstdlib>
#include <vector>

#include "q1buffer.h"

//change else check




bool convert( int &val, char *buffer ) {		// convert C string to integer
	std::stringstream ss( buffer );			// connect stream and buffer
	ss >> dec >> val;					// convert integer from buffer
	return ! ss.fail() &&				// conversion successful ?
		// characters after conversion all blank ?
		string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
} // convert

enum { sizeDeflt = 20, codeDeflt = 5 };			// global defaults

void usage( char *argv[] ) {
	cerr << "Usage: " << argv[0]
		<< "[ Cons (> 0) [ Prods (> 0) [ Produce (> 0) [ BufferSize (> 0) [ Delay (> 0) ] ] ] ] ]" << endl;
	exit( EXIT_FAILURE );				// TERMINATE
} // usage

void uMain::main() {

	//variable decleration
	int Cons = 5; 
	int Prods = 3; 
	int Produce = 10; 
	int BufferSize=10;
	int sum=0;
	int Delay = Cons+Prods;
	int SENTINEL = -1;

	//tracks all producers
	vector<Producer*> allProducers;
	//tracks all consumer
	vector<Consumer*> allConsumers;

	//used for input checking
	int code;

	//check all arguments
	switch ( argc ) {
		
		case 6: //set delay
			if ( ! convert( code, argv[5] ) || code < 0 ) {	// invalid integer ?
				usage( argv );
			}

			Delay = code;
		
		case 5: //set buffer size	
			if ( ! convert( code, argv[4] ) || code < 0 ) {	// invalid integer ?
				usage( argv );
			}

			BufferSize = code;
			
		case 4: //set amount of produce per producer
			if ( ! convert( code, argv[3] ) || code < 0 ) {	// invalid integer ?
				usage( argv );
			} 

			Produce = code;
		
		case 3: //set number of producers
			if ( ! convert( code, argv[2] ) || code < 0 ) {	// invalid integer ?
				usage( argv );
			}

			Prods = code;
		case 2: //set number of consumers
			if ( ! convert( code, argv[1] ) || code < 0 ) {	// invalid integer ?
				usage( argv );
			}

			Cons = code;
		case 1:
			break;
		default:						// wrong number of options
			usage( argv );
	}
	
	#ifdef __U_MULTI__
		uProcessor p[3] __attribute__(( unused ));
	#endif

	//create a boundedbuffer
	BoundedBuffer<int> b1(BufferSize);

	//create all producers
	for(int i=0; i<Prods; i++){
		Producer *p1 = new Producer(b1, Produce, Delay);
		allProducers.push_back(p1);
	}

	//create all consumers
	for(int i=0; i<Cons; i++){
		Consumer *c1 = new Consumer(b1,Delay,-1,sum);
		allConsumers.push_back(c1);
	}

	//delete all producers
	for(int i=0; i<Prods; i++){
		delete allProducers.at(0);
		allProducers.erase(allProducers.begin());
	}

	//set sentinel value, -1
	for(int i=0; i<Cons; i++){
		b1.insert(SENTINEL); //sentinel value
	}

	//delete consumers
	for(int i=0; i<Cons; i++){
		delete allConsumers.at(0);
		allConsumers.erase(allConsumers.begin());
	}

	//display total
	cout << "total: " << sum << endl;

}
