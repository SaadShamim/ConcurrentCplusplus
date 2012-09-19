#include <uC++.h>
#include "q2PRNG.h"
#include "q2Printer.h"
#include "q2Philosopher.h"
#include "q2table.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdlib>

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
	int P = 5; 
	unsigned int N = 30; 

switch ( argc ) {
		case 4: //seed
			if ( ! convert( code, argv[3] ) || code < 1 ) {	// invalid integer ?
				usage( argv );
			}	
			seedVal = code;
		case 3: //N
			if ( ! convert( code, argv[2] ) || code < 1 ) {	// invalid integer ?
				usage( argv );
			}	
			N= code;
		case 2: //P
			if ( ! convert( code, argv[1] ) || code < 2 ) {	// invalid integer ?
				usage( argv );
			} 		
			P = code;		
		case 1:
			break;
		default:						// wrong number of options
			usage( argv );
	}


	cout << seedVal << endl;
	
	myPrng.seed(seedVal);
	vector<Philosopher*> allPhilosophers;
	Printer p1 (P);
	Table t1(P, p1);

	

	for(unsigned int i=0; i<P; i++){
		Philosopher *p = new Philosopher(i,N,t1,p1);
		allPhilosophers.push_back(p);
	}
	
	for(int i=0; i<P; i++){
		delete allPhilosophers.at(0);
		allPhilosophers.erase(allPhilosophers.begin());
	}
	
}
