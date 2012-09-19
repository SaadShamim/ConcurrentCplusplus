#include <uC++.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;					// direct access to std
#include <cstdlib>
#include <vector>

#include "q2Printer.h"
#include "q2tallyVotes.h"
#include "q2Voter.h"
#include "q2PRNG.h"

#include <uSemaphore.h>

PRNG myPrng;

enum { codeDeflt = 5 };			// global defaults

bool convert( int &val, char *buffer ) {		// convert C string to integer
	std::stringstream ss( buffer );			// connect stream and buffer
	ss >> dec >> val;					// convert integer from buffer
	return ! ss.fail() &&				// conversion successful ?
	// characters after conversion all blank ?
	string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
} // convert

void usage( char *argv[] ) {
	cerr << "Usage: " << argv[0]
		<<"Voters (> 0 & V mod G = 0, default 6)  Group (> 0 & odd, default 3)  Seed (> 0)"<< endl;
	exit( EXIT_FAILURE );				// TERMINATE
} // usage	

void uMain::main() {
	
	int code = codeDeflt;		// default value

	uint32_t seedVal = getpid();
	int V = 6;
	unsigned int G = 3;

	switch ( argc ) {
		//seed provided
		case 4:
			if ( ! convert( code, argv[3] ) || code < 1 ) {	// invalid integer ?
				usage( argv );
			}
			
			seedVal = code;

		case 3: //group provided
			if ( ! convert( code, argv[2] ) || code < 0 ) {	// invalid integer ?
				usage( argv );
			}
			
			//make sure its odd
			G = code;
			if(G % 2 == 0)
				usage( argv );

		case 2: //visitors provided
			if ( ! convert( code, argv[1] ) || code < 0 ) {	// invalid integer ?
				usage( argv );
			} 
			
			//check if voters and group mem are evenly divisble
			V = code;
			if(V%G != 0)
				usage( argv );
				
		case 1:
			break;
		default:						// wrong number of options
			usage( argv );
	}	
	
	myPrng.seed(seedVal);

	vector<Voter*> allVoters;

	Printer p1 (V); //create printer
	TallyVotes tv(G, p1); //create tallyVotes
	
	
	//create voters
	for(int i=0; i<V; i++){
		Voter *v1 = new Voter(i,tv,p1);
		allVoters.push_back(v1);
	}
	
	//delete voters
	for(int i=0; i<V; i++){
		delete allVoters.at(0);
		allVoters.erase(allVoters.begin());
	}
}
