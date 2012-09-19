#include "q2Printer.h"
#include "q2Voter.h"
#include <iostream>

using namespace std;

void Printer::print( unsigned int id, Voter::States state ){
	if(bufferArray[id] == 0){
			//print S
		bufferArray[id] = state;	
	}else{
		clearBuffer();
		bufferArray[id] = state;
	}
	
	/*addSpacing(id);
	cout << (char)bufferArray[id] << id;
	if(id==numberOfVoters-1)
		cout << endl;*/
}	

void Printer::print( unsigned int id, Voter::States state, bool vote ){
	if(bufferArray[id] == 0){
			//print S
		bufferArray[id] = state;	
	}else{
		clearBuffer();
		bufferArray[id] = state;
	}

	if(state == Voter::Finished){
		for(int i=0;i<numberOfVoters;i++){
			if(id != i)
				cout << "...";
			else
				cout << (char)state << " " << vote;
			cout << "\t";
			bufferArray[i] = 0;
		}
		cout << endl;
	}else{ 
	
	/*addSpacing(id);
	cout << (char)bufferArray[id] << " " << id;
	if(id==numberOfVoters-1)
			cout << endl;*/
			}
}

void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ){
	if(bufferArray[id] == 0){
			//print S
		bufferArray[id] = state;	
	}else{
		clearBuffer();
		bufferArray[id] = state;
	}
	
	/*	addSpacing(id) ;
		cout << (char)bufferArray[id] << " " << numBlocked ;
		if(id==numberOfVoters-1)
			cout << endl;*/
	

}

Printer::Printer( unsigned int voters ):numberOfVoters(voters),bufferArray(new int[voters]){
	for(int i=0; i<voters;i++){
		cout << "Voter" << i << "\t";
		bufferArray[i] = 0;
		
	}
	cout << endl;
	for(int i=0; i<voters;i++){
		cout << "=======" << "\t";
	}
	
	cout << endl;
}

void Printer::clearBuffer(){
	for(int i=0; i<numberOfVoters;i++){
		if(bufferArray[i] != 0){
			cout << (char)bufferArray[i];
		}else{
			cout << "   ";
		}
		cout << "\t";
		bufferArray[i] = 0;
	}
	cout << endl;
}

void Printer::addSpacing(int id){
	int lastid=0;
	for(int i=id-1; i>0; i--){
		if(bufferArray[i] != 0){
			lastid = i;
			break;
		}
	}
	
	//cout << "last id: " << lastid << "id " << id << "aaaa" << (id-lastid) << endl;
	for(int i = 0; i < (id-lastid); i++){
		cout << "\t";
	}

	/*for(int i=0;i<id;i++){
		if(bufferArray[i] == 0){
			cout << "   ";
		//}else {
		//	cout << (char)bufferArray[i];
		}
		cout << "\t";
	}*/
}
