#include <uC++.h>
#include <cstdlib>
#include "q2Workshop.h"
#include "q2Santa.h"
#include "q2Elf.h"
#include "q2PRNG.h"
#include <vector>
#include "q2Printer.h"
#include "q2Reindeer.h"
#include <sstream>
#include <iostream>

using namespace std;

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
		<<" [ N (>= 0) [ E (>= 0) [ seed (> 0) [ C (> 0) [ D (> 0) ] ] ] ] ]"<< endl;
	exit( EXIT_FAILURE );				// TERMINATE
} // usage	

void uMain::main() {

	int code = codeDeflt;

	uint32_t seedVal = getpid();
	int N = 3; //bound for reindeer
  	unsigned int E = 3; //number of elves
	int C = 3; //number of time each elf consults
	unsigned int D = 3; //number of time each reindeer delivers
	
	switch ( argc ) {
	//D
		case 6:
			if ( ! convert( code, argv[5] ) || code < 0 ) {	// invalid integer ?
				usage( argv );
			}
			
			D = code;
	
		case 5:
			if ( ! convert( code, argv[4] ) || code < 0 ) {	// invalid integer ?
				usage( argv );
			}
			
			C 	= code;
	
		//C
		case 4:
			if ( ! convert( code, argv[3] ) || code < 1 ) {	// invalid integer ?
				usage( argv );
			}
			
			seedVal = code;
		//Seed
		case 3: //group provided
			if ( ! convert( code, argv[2] ) || code < 1 ) {	// invalid integer ?
				usage( argv );
			}
			
			E = code;

		case 2: //visitors provided
			if ( ! convert( code, argv[1] ) || code < 1 ) {	// invalid integer ?
				usage( argv );
			} 
			
			N = code;
				
		case 1:
			break;
		default:						// wrong number of options
			usage( argv );
	}
	
	myPrng.seed(seedVal);
	
	//track all elves and reindeers
	vector<Elf*> allElves;
	vector<Reindeer*> allReindeers;
	
	//create printer
	Printer p1 (E);
	//create workshop, monitor
	Workshop w1(p1, E, N, D);
	//create santa
	Santa s1(w1, p1);
	
	//create all elves
	for(int i=1; i<E+1; i++){
		Elf *e1 = new Elf(i,w1,p1,C);
		allElves.push_back(e1);
	}
	
	//create all reindeers
	for(unsigned int i=E+1; i<Reindeer::MAX_NUM_REINDEER+E+1; i++){
		Reindeer *r1 = new Reindeer(i,w1,p1,D);
		allReindeers.push_back(r1);
	}
	
	//delete all elves
	for(int i=0; i<E; i++){
		delete allElves.at(0);
		allElves.erase(allElves.begin());
	}
	
	//delete all reindeers
	for(int i=0; i<Reindeer::MAX_NUM_REINDEER; i++){
		delete allReindeers.at(0);
		allReindeers.erase(allReindeers.begin());
	}
	
}
