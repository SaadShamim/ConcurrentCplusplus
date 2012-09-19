#include <uC++.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;					// direct access to std
#include <cstdlib>					// exit
#include <cmath>
#include "q2quicksort.h"

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
	 << ") [ input-file [ output-file || -depth ] ] ] ]" << endl;
    exit( EXIT_FAILURE );				// TERMINATE
} // usage

void uMain::main() {
	
	int code = codeDeflt;		// default value
    
    TYPE *a;
    int arraySize; //array size
    unsigned int depth=1; //inital depth value
    bool depthAvailable; //to check if a depth value was put in (incase user puts in 1 as depth)
    
    
    istream *infile = &cin;				// default value
    ostream *outfile = &cout;				// default value

	switch ( argc ) {
		case 3:
			//if - aviable as second arg, expects number else file
			if(argv[2][0] == '-'){
						
				if ( ! convert( code, argv[2]+1 ) || code < 1 ) {	// invalid integer ?
					usage( argv );	
				} 		
				//set depth	
				depth = code;
				depthAvailable = true;
				
			}else{
				try {
					outfile = new ofstream( argv[2] );
				} catch( uFile::Failure ) {			// open failed ?
					cerr << "Error! Could not open output file \"" << argv[2] << "\"" << endl;
					usage( argv );
				}		

			}
		case 2:
			//check for valid input file
			try {
				infile = new ifstream( argv[1] );
			} catch( uFile::Failure ) {
				cerr << "Error! Could not open input file \"" << argv[2] << "\"" << endl;
				usage( argv );
				
			} // try
			break;
		default:						// wrong number of options
			usage( argv );
			
	 } // switch

	uProcessor processors[ depth - 1 ]__attribute__(( unused )); // use “depth” kernel threads
	
	
	for(;;){
		*infile >> arraySize;					// read arraySize
		
		//base case, read until faile finshed	
		if ( infile->fail()) break;		// failed ?		
		
		//safely allocate memory	
		a = (TYPE *) malloc(sizeof(TYPE) * arraySize);
		if (a == NULL) {
			cerr << "Out of memory" << endl;
			exit(1);
		}
		//varaible used to read data
		TYPE blah;
		
		//if no depth available read in from file
		if(!depthAvailable){
			for (int i = 0; i < arraySize; i++) {
				if(i!=0 && i%25==0){ //used for space allignment
					if(!depthAvailable)
					*outfile << endl << " " << " ";
				}
				
				*infile >> blah;
				
				//output data
				if(!depthAvailable)
				*outfile << blah << " ";			
				a[i] = blah;
			}
		//if depth, read in first value and generate random array
		}else{
			int val = arraySize;
			for(int i= 0; i < arraySize; i++, val--){
				a[i] = val;
			}
		}
		
		if(!depthAvailable)
			*outfile << endl;
		
		//creates quicksort process	
		{
			QuickSort<TYPE> q1(a,0,arraySize-1, log2(depth));
		}//wait for return
		
		//output sorted values to file our console
			for(int i = 0; i < arraySize; i++){
				if(i!=0 && i%25==0){
					if(!depthAvailable)
						*outfile << endl << " " << " ";
				}
				if(!depthAvailable)
				*outfile << a[i] << " ";	
			} 
			
		if(!depthAvailable)
		 *outfile << endl << endl;
		//free mem
		free(a);								// dealocate memory	
		
	}
	
	if ( infile != &cin ) delete infile;		// close file, do not delete cin!
    if ( outfile != &cout ) delete outfile;		// close file, do not delete cout!
  
} // uMain::main

