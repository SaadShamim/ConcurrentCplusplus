#include "station.h"
#include <vector>
#include <iostream>
#include "PRNG.h"

vector<Station *> Station::active;
unsigned int Station::value;
Station* Station::coordinator;

extern PRNG myPrng;

///MAKE VALUE GLOBAl
//iFailed to failed()
//remove starterFlag

///MAKE VALUE GLOBAl
//iFailed to failed()
//remove starterFlag
//change enum
//55 179 977


// 89 sends 23
// ./tokenring 18 11 32 > me.out


void Station::main() {
	//	printf("derp");
	holdCounter = myPrng(1);
	//cerr << idx << " " << holdCounter << endl;
	suspend();
	Station *This = this;
	
	try {
		
		_Enable {   
			//move this UPPP
			
			// allow delivery of nonlocal exceptions
			// establish starter for termination
			
			// _Enable
			while(true){
				
				//basecase
				
				//cerr << "start while" << endl;
				
				if(myPartner->idx == idx){
					//cerr << "breaking" << endl;
					//	break;
				}
				
				//cerr << "outside break" << endl;
				
				while(myPartner->failed()){
					cout << idx << "(fail " << myPartner->idx << ", part " << (myPartner->partner())->id() << ") "; 
					
					
					if(myPartner->idx == coordinator->idx){
						//cerr << "****************call election" << endl;
						cout << idx << "(elec";
						starterFlag = idx; //erase later, check ur coordinator==partner instead
						_Resume Election(*this) _At *(myPartner->partner());
						myPartner->partner()->vote();
					}
					
					if(fdstId == myPartner->idx){
						if(fFrameType == ACK){
							//convert to token
							cout << idx << "(drop Ack)" << endl;
							fFrameType = TOKEN;
							fdstId = idx;
						}else if (fFrameType == NACK){
							cout << idx << "(drop Nack)" << endl;
							fFrameType = TOKEN;
							fdstId = idx;
						} else if(fFrameType == DF){
							cout << idx << "(drop Data)";
							if(fsrcId == fdstId){
								cout << endl;
								//covert to token
								fFrameType = TOKEN;
								fdstId = idx;
							}else{
								cout << " ";
								//convert to nack
								fFrameType = NACK;
								fdstId = fsrcId;
								fsrcId = idx;
							}
							
						}
					} else if ( fdstId == idx && fsrcId == myPartner->id()) {
						if ( fFrameType == DF ) {
							cout << idx << "(rec " << value << ", from " << fsrcId << ") "; 
							cout << idx << "(drop Ack)" <<endl;
						}
					}
					
					/*	if(myPartner->idx == coordinator->idx){
					 //cerr << "****************call election" << endl;
					 cout << idx << "(elec";
					 starterFlag = idx; //erase later, check ur coordinator==partner instead
					 _Resume Election(*this) _At *(myPartner->partner());
					 myPartner->partner()->vote();
					 
					 
					 
					 
					 //cerr << "new coordinator: " << coordinator->idx << endl;
					 
					 
					 /*	try {
					 _Enable {   
					 //move this UPPP
					 
					 // allow delivery of nonlocal exceptions
					 suspend();                    // establish starter for termination
					 } // _Enable */
					
					/*} _CatchResume(Election &election) (Station *This)    {
					 //deal with election
					 if((election.candidate).id() > This->id() || This->iFailed){
					 _Resume Election(election.candidate) _At *(This->partner());
					 }else{
					 _Resume Election(*This) _At *(This->partner());
					 }
					 (This->partner())->vote();
					 } // try*/
					//}
					// cerr << "calling delete" << endl;	
					myPartner = coordinator->remove(myPartner);
					/*if(fFrameType == DF){
					 break;
					 }*/
				}
				
				//my partner is active
				
				
				
				//cerr << "partner active" << endl;
				//generate failed random number
				//int x = myPrng((failurex-1));
				if(myPrng((failurex-1)) == 0){
					//cout << idx << " will fail" << endl;
					iFailed = true;
				}
				
				
				
				if(myPartner->idx == idx){
					//	break;
				}
				
				
				Frame tmpFrame;
				//cerr << "heading into active" << endl;	
				//analyize frame
				if((fFrameType==DF) && (fdstId == idx) && (check(fsrcId) == false)){	
					//cerr << idx << ", " << tmpFrame.dstId <<endl;
					cout << idx << "(drop Ack)" << endl;
					fFrameType = TOKEN;
					fdstId = idx;
				}
				
				if(fFrameType == TOKEN){
					//cerr << "have token" << endl;
					//should I hold or release token?
					if(holdCounter == 0){
						//reset counter
						holdCounter = myPrng(1,3);
						value++;
						int dstIndex = myPrng((active.size()-1));
						//create dataframe
						tmpFrame.type = DF;
						tmpFrame.value = value;
						tmpFrame.srcId = idx;
						tmpFrame.dstId =  active[dstIndex]->idx;
						//send dataframe
						cout << idx << "(send " << value << ", to " << active[dstIndex]->idx << ") ";
						myPartner->transmit(tmpFrame);
					}else{
						cout << idx << "(skip) ";
						holdCounter--;
						tmpFrame.type = TOKEN;
						tmpFrame.value = value;
						tmpFrame.srcId = idx; //doesn't matter, never checked
						tmpFrame.dstId = myPartner->idx; //doesn't matter never checked
						myPartner->transmit(tmpFrame);
					}	
				} else if(fFrameType == NACK || fFrameType==ACK){
					//cerr << "have NACK" << endl;
					if(fdstId == idx){
						tmpFrame.type = TOKEN;
						tmpFrame.value = value;
						tmpFrame.srcId = idx; 
						tmpFrame.dstId = myPartner->idx; 
						
						if(fFrameType == ACK){
							cout << idx << "(ack)"<< endl;
						}else{
							cout << idx << "(nack)"<<endl;
						}
						
						
						myPartner->transmit(tmpFrame);
					}else{
						//pass frame
						
						if(fFrameType == ACK){
							cout << idx << "(ack " << fdstId << ") ";
						}else{
							cout << idx << "(nack " << fdstId << ") ";
						}
						
						tmpFrame.type = fFrameType;
						tmpFrame.value = fValue;
						tmpFrame.srcId = fsrcId; //doesn't matter, never checked
						tmpFrame.dstId = fdstId; //doesn't matter never checked
						myPartner->transmit(tmpFrame);
					}
				} else if(fFrameType == DF){
					//cerr << "have dataframe" << endl;
					if(fdstId == idx){
						cout << idx << "(rec " << value << ", from " << fsrcId << ") "; 
						tmpFrame.type = ACK;
						tmpFrame.value = value;
						tmpFrame.srcId = idx; 
						tmpFrame.dstId = fsrcId; 
						myPartner->transmit(tmpFrame);
					}else{//pass on the data frame
						cout << idx << "(for " <<  fdstId << ", " << value << ") "; 
						tmpFrame.type = fFrameType;
						tmpFrame.value = fValue;
						tmpFrame.srcId = fsrcId; 
						tmpFrame.dstId = fdstId; 
						myPartner->transmit(tmpFrame);
					}
				}
				
				//cerr << "end while" << endl;
				
				if(myPartner->idx == idx){
					//cerr << "breaking" << endl;
					break;
				}
				
			}//end while
			
		} //close Enable
	} _CatchResume(Election &election) (Station *This)    {
		//deal with election
		cout << " ";
		if(This->partner() != Station::coordinator){
			if(This->failed()){
				cout << election.candidate.id() << ">" << This->id();
				_Resume Election(election.candidate) _At *(This->partner());
			}else if((election.candidate).id() > This->id()){
				cout << election.candidate.id() << "?" << This->id();
				_Resume Election(election.candidate) _At *(This->partner());	
			}else{
				cout << election.candidate.id() << "?" << This->id();
				_Resume Election(*This) _At *(This->partner());
			}
			
			(This->partner())->vote();
		}else{
			cout << "win " << election.candidate.id() << ") ";
			(Station::coordinator) = &(election.candidate);
			
		}
	} // try
	
	//	cerr << "coordinators id: " << coordinator->idx << "my id: " << idx << "vector size: " << active.size() << endl;
	//	cerr << "Now returning (hopefully to the real main)" << endl;
	//	suspend();
	
	cout << "no more partners"<<endl;
	return;
}

bool Station::check(int dst){
	
	for(int i=0;i<active.size();i++){
		if(active[i]->id() == dst){
			return true;
		}
	}
	//cerr << "checked" << endl;
	return false;
}

Station::Station(unsigned int id , unsigned int failure) : idx(id),failurex(failure){
	//cerr << "Constructor ID: " << idx << "Given ID: " << id << endl;
	active.push_back(this);
	//cout <<	active.size() << endl;
	
}

unsigned int Station::id(){
	return idx;
}

bool Station::failed(){
	return iFailed;
}

void Station::transmit(const Frame &f){
	//cerr << "starting transmit: " << idx << endl;
	fFrameType = f.type;
	fValue = f.value;
	fsrcId = f.srcId;
	fdstId = f.dstId;
	//cerr << "Activity Size" << active.size() << endl;
	//cerr << fFrameType << fValue << fsrcId << fdstId << myPartner->idx << endl;
	resume();
	
	//cerr << "Activity Size" << active.size() << endl;
	//cerr << "ending transmission: " << idx << endl;
	//cerr << "Came out of back of resume in transmit properly" << endl;
}

void Station::start(Station *partner){
	iFailed = false;
	//cerr << "initial iFailed:" << iFailed << endl;
	//cerr << "id: " << idx << " holdCounter: " << holdCounter << endl;
	myPartner = partner;
	resume();
	//cout << idx << "Partner ID: " << myPartner->idx << endl;
}

Station* Station::remove(Station *victim){
	/*cerr <<"removing():" << endl;
	 cerr << "current: " << idx << " removing: " << victim->idx << endl;
	 cerr << "coordinator: " << coordinator->idx << endl;
	 cerr << "active size: " << active.size() << endl;*/
	//cerr << "removing" << endl;
	Station *newPartner;
	for(unsigned int i=0; i<active.size(); i++){
		if(active[i]->idx == victim->idx){
			newPartner = active[i]->partner();
			//delete active[i];
			//cerr << "should delete at index: " << i << endl;
			
			/*for(int j=0;j<active.size();j++){
			 cerr << active[j]->id() << ", ";
			 }*/
			//cerr << endl;
			active.erase(active.begin()+i);
			//	cerr << "after delete: " << endl;	
			/*for(int j=0;j<active.size();j++){
			 cerr << active[j]->id() << ", ";
			 }*/
			//cerr << endl;
			break;
		}
	}
	delete victim;	
	//cerr << "active size: " << active.size() << endl;
	return newPartner; 
}

Station* Station::partner(){
	return myPartner;
}



void Station::vote(){
	resume();
	//cerr << "after resume in vote, current id: " << idx << endl;
}

