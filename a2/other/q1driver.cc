#include "station.h"
#include <uC++.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;					// direct access to std
#include <cstdlib>					// exit
#include "PRNG.h"
#include <vector>
#include <unistd.h>

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
	 << " [ size (>= 0 : " << sizeDeflt
	 << ") [ code (>= 0 : " << codeDeflt
	 << ") [ input-file stations [failure[seed]] ]" << endl;
    exit( EXIT_FAILURE );				// TERMINATE
} // usage

PRNG myPrng;

void uMain::main() {
	int size = sizeDeflt, code = codeDeflt;		// default value
	uint32_t seedVal = getpid();
	int stationNumber=0; //number of stations
	int F = 15; //default failure values
	vector<Station *> stationsCreated;     // global variable, active stations

	//check all arguments
	switch ( argc ) {
		//seed provided
		case 4:
			if ( ! convert( code, argv[3] ) || code < 0 ) {	// invalid integer ?
				usage( argv );
			}
			
			seedVal = code;
		//failure value provided
		case 3:
			if ( ! convert( code, argv[2] ) || code < 1 ) {	// invalid integer ?
				usage( argv );
			}
			F = code;
		//number of stations provided
		case 2:
			if ( ! convert( size, argv[1] ) || size < 1 ) {	// invalid integer ?
				usage( argv );
			} 
			stationNumber = size;
			break;
		default:						// wrong number of options
			usage( argv );
	}
	myPrng.seed(seedVal);
	
	//create number of stations as provided by user	
	for(int i=0; i<stationNumber; i++){
		stationsCreated.push_back(new Station(i,F));
	}
	//initialize stations
	for(int i=stationsCreated.size()-2; i>=0; i--){
		stationsCreated[i]->start(stationsCreated[i+1]);
	}
	//create cycle
	stationsCreated[stationsCreated.size()-1]->start(stationsCreated[0]);
	
	//make token to pass to first station
	Station::Frame tempFrame;
	tempFrame.type = Station::Frame::TOKEN;
	tempFrame.value = 0;
	tempFrame.srcId = 0; 
	tempFrame.dstId = 0;
	
	//set station 0 as coordinator
	Station::coordinator = stationsCreated[0];
	stationsCreated[0]->transmit(tempFrame);
	delete Station::coordinator; //delete last station
	
} // uMain::main
